#pragma once

#include <string_view>
#include <cstdint>
#include <span>
#include <type_traits>

struct RslV3
{
    float x, y, z;
};

struct RslQuat
{
    float x, y, z, w;
};

struct RslMatrix
{
    RslV3 right;
    float pad0;
    RslV3 up;
    float pad1;
    RslV3 at;
    float pad2;
    RslV3 pos;
    float pad3;

    void
    Scale (float x, float y, float z)
    {
        right.x *= x;
        up.x *= x;
        at.x *= x;

        right.y *= y;
        up.y *= y;
        at.y *= y;

        right.z *= z;
        up.z *= z;
        at.z *= z;
    }
};

enum eRslObjectType
{
    RSL_OBJECT_NODE = 0,
    RSL_OBJECT_HAS_NODE = 1,
    RSL_OBJECT_ELEMENT_GROUP = 2,
    RSL_OBJECT_TEX_LIST = 6,
    RSL_OBJECT_GEOMETRY = 8
};

template <typename T> struct RslLLLink
{
    struct RslLLLink *next;
    struct RslLLLink *prev;

    T operator * ()
    {
        return reinterpret_cast<T> (reinterpret_cast<uintptr_t> (this)
                                    - offsetof (std::remove_pointer_t<T>, lLink));
    }

    struct iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T *;
        using reference         = T &;

        RslLLLink *ptr;

        iterator &operator++ ()
        {
            ptr = ptr->next;
            return *this;
        }

        iterator &operator-- ()
        {
            ptr = ptr->prev;
            return *this;
        }

        T
        operator* ()
        {
            return **ptr;
        }

        bool operator== (const iterator &other) const
        {
            return ptr == other.ptr;
        }

        bool operator!= (const iterator &other) const
        {
            return ptr != other.ptr;
        }
    };
};

template<typename T>
struct RslLinkList
{
    struct RslLLLink<T> link;

    RslLinkList ()
    {
        link.next = &link;
        link.prev = &link;
    }

    auto
    begin ()
    {
        return typename RslLLLink<T>::iterator {link.next};
    }

    auto
    end ()
    {
        return typename RslLLLink<T>::iterator {&link};
    }

    void
    insert (T node)
    {
        node->lLink.next = link.next;
        node->lLink.prev = &link;
        link.next->prev = &node->lLink;
        link.next = &node->lLink;
    }
};

template<int Type, typename T = void*>
struct RslObject {
    uint8_t  type;
    uint8_t  subType;
    uint8_t  flags;
    uint8_t  privateFlags;
    T       *data;

    RslObject ()
        : type (Type), subType (0), flags (0), privateFlags (0), data (nullptr)
    {
    }
};

struct RslObjectHasNode : public RslObject<1, class RslNode>
{
    struct RslLLLink<class RslNode *> lLink;
    void (*sync) ();
};

struct RslRaster
{
    void    *commandBuffer = nullptr;
    uint8_t *data = nullptr;
    int16_t  stride = 0;
    uint8_t  logWidth = 0;
    uint8_t  logHeight = 0;
    uint8_t  depth = 0;

    uint8_t mipmaps : 6 = 0;
    bool    uClamp : 1 = 0;
    bool    vClamp : 1 = 0;

    uint8_t unk = 0;

    enum ColorMode
    {
        GU_PSM_5551 = 1,
        GU_PSM_8888 = 5,
    } colorMode : 5 = GU_PSM_5551;

    enum PaletteMode
    {
        NONE,
        GU_PSM_T8,
        GU_PSM_T4,


    } paletteMode : 3 = NONE;

    uint32_t
    GetWidth ()
    {
        return 1 << logWidth;
    }

    uint32_t
    GetHeight ()
    {
        return 1 << logHeight;
    }

    uint32_t DataSize ()
    {
        uint32_t main_size = GetTexel(mipmaps) - data;
        uint32_t pal_size = CalculatePaletteSize() * 4;
        return main_size + pal_size;
    }
    
    uint8_t *
    GetTexel (int32_t n)
    {
        auto     out = data;
        uint32_t w   = stride;
        uint32_t lH  = logHeight;

        while (n--)
            {
                out += w * (1 << lH);
                lH--;

                if (0x10 < w)
                    w /= 2;
            }

        return out;
    }

    uint8_t
    GetPaletteIdx (size_t x, size_t y)
    {
        uint32_t w = GetWidth ();
        uint32_t h = GetHeight ();

        if (x >= w || y >= h)
            return 0;

        switch (paletteMode)
            {
            case NONE: return 0;
            case GU_PSM_T4: return data[y * w + x];
            case GU_PSM_T8: return data[(y * w + x) / 2] >> (4 * (x & 1)) & 0xf;
            }

        return 0;
    }

    uint16_t
    CalculatePaletteSize ()
    {
        switch (paletteMode)
            {
            case GU_PSM_T8: return 0x100;
            case GU_PSM_T4: return 0x10;
            default: return 0;
            }
    }

    uint32_t
    GetPixelColour (size_t x, size_t y)
    {
        if (depth <= 8)
            {
                uint8_t   idx     = GetPaletteIdx (x, y);
                uint32_t *palette = GetPalettePtr ();
                return palette ? palette[idx] : 0;
            }

        return 0;
    }

    uint32_t *
    GetPalettePtr ()
    {
        if (depth <= 8)
            return reinterpret_cast<uint32_t *> (GetTexel (mipmaps));

        return nullptr;
    }

  auto
  GetPalette ()
  {
    return std::span (GetPalettePtr (), CalculatePaletteSize ());
  }
};

class RslTAnimNodeInfo {
public:
    int8_t      id;
    int8_t      index;
    int8_t      flags;
    RslNode  *frame;
};

class RslNode : public RslObject<RSL_OBJECT_NODE>
{
public:
    RslLinkList<RslObjectHasNode *> objectList;
    RslMatrix                       modelling;
    RslMatrix                       ltm;
    RslNode                        *child;
    RslNode                        *next;
    RslNode                        *root;

    // union
    // {
    //     struct
    //     {
    //         int32_t             nodeId;
    //         class RslTAnimTree *animTree;
    //         char               *name;
    //         int32_t             nodeTreeId;
    //     } extensionLCS;

    //     struct
    //     {
            int32_t             nodeId;
            int32_t             unk;
            class RslTAnimTree *animTree;
            char               *name;
            int32_t             nodeTreeId;
    //     };
    // };

    template<typename F>
    void
    ForAllChildren (F &&functor)
    {
        auto node = child;

        while (node)
            {
                functor (node);
                node = node->next;
            }
    }

    template<typename F>
    void
    ForAllChildrenRecursive (F &&functor, int depth = 0)
    {
        auto node = child;

        while (node)
            {
                functor (node, depth);
                node->ForAllChildrenRecursive (functor, depth + 1);
                node = node->next;
            }
    }

    RslNode* FindChildByName (std::string_view name)
    {
        RslNode* node = nullptr;

        ForAllChildrenRecursive ([&] (RslNode* n, int) {
            if (n->name && n->name == name)
                node = n;
        });

        return node;
    }

    void
    AddChildAtEnd (RslNode *node)
    {
        if (!child)
            {
                child = node;
                return;
            }

        auto last = child;

        while (last->next)
            last = last->next;

        last->next = node;
    }
};

class RslTAnimTree
{
public:
    int32_t flags;
    int32_t numNodes;
    void   *pCurrentAnim;
    float   currentTime;
    void   *pNextFrame;
    void (*pAnimCallBack) ();
    void *pAnimCallBackData;
    float animCallBackTime;
    void (*pAnimLoopCallBack) ();
    void             *pAnimLoopCallBackData;
    RslMatrix        *pMatrixArray;
    RslMatrix        *pMatrixArrayUnaligned;
    RslTAnimNodeInfo *pNodeInfo;
    RslNode          *parentNode;

    struct KeyFrame
    {
        uint32_t unk;
        uint32_t unk2;
        RslQuat quat;
        RslV3 pos;
    };

    union
    {
        struct
        {
            int32_t  maxKeyFrameSize;
        int32_t  currentKeyFrameSize;
        void (*keyFrameToMatrixCB) ();
        void (*keyFrameBlendCB) ();
        void (*keyFrameInterpolateCB) ();
        void (*keyFrameAddCB) ();
        RslTAnimTree *parentTree;
        int32_t       offsetInParent;
        int32_t       rootParentOffset;
        uint32_t      keyFrameSize;
        uint8_t       keyframeData[];
        } lcsExtension;

        struct
        {
            KeyFrame keyframeData[];
        };
    };
};

class RslMaterialList {
public:
    class RslMaterial** list;
    uint32_t numMaterials;
    uint32_t space;
};

class RslSkin
{
public:
    uint32_t numBones;
    uint8_t  unk1[8];
    RslMatrix *pBoneMatrices;
};

struct TriangleList
{
    uint32_t offset;
    uint16_t numVerts;
    uint16_t material;
    float textureSlopeMultiplier;
    float vScale;
    float uScale;
    char unk[20];
    uint8_t boneIdx[8];
};

struct VertexBuffer
{
    uint32_t size;
    uint32_t vertexType;
    uint32_t numLists;
    uint8_t unk[52];
    uint32_t vertDataOffset;
    uint32_t unk2;
    TriangleList triList[];
};

class RslMaterial
{
public:
    union
    {
        struct RslTexture* texture;
        char* textureName;
    };
    uint32_t color;
    uint32_t byteCount;
    class RslMatfx* matfx;
};

class RslGeometry : public RslObject<RSL_OBJECT_GEOMETRY>
{
public:
    uint16_t numRefs;
    uint16_t unk1;
    RslMaterialList materials;
    RslSkin* skin;
    uint32_t unk2;
    VertexBuffer data;
};

class RslElement : public RslObjectHasNode
{
public:
    class RslGeometry            *geometry;
    class RslElementGroup        *elementGroup;
    class RslLLLink<RslElement *> lLink;
    void                         *renderCallback;
    uint16_t                      modelId;
    uint16_t                      unk_0x2A;
    RslTAnimTree                 *animTree;
    uint32_t                      unk_0x30;
};

class RslElementGroup : public RslObject<RSL_OBJECT_ELEMENT_GROUP, RslNode>
{
public:
    struct RslLinkList<RslElement *> elements;
    uint32_t unk_0x10;
    uint32_t unk_0x14;
    class CAnimBlendElementGroupData* animBlendGroupData;
};

static_assert (sizeof (RslRaster) == 16, "RslRaster size mismatch");

struct RslTexture
{
    struct RslRaster              *raster = nullptr;
    struct RslTexList             *dict   = nullptr;
    struct RslLLLink<RslTexture *> lLink;
    char                           name[32] = "";
    char                           mask[32] = "";
};

struct RslTexList : public RslObject<RSL_OBJECT_TEX_LIST>
{
    RslLinkList<RslTexture *> textures;
    RslLLLink<RslTexList *>   lLink;
};
