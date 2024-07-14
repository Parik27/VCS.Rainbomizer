#pragma once

#include <algorithm>
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
};

template<typename T = void*>
struct RslObject {
    uint8_t  type;
    uint8_t  subType;
    uint8_t  flags;
    uint8_t  privateFlags;
    T       *data;
};

struct RslObjectHasNode : public RslObject<class RslNode>
{
    struct RslLLLink<class RslNode *> lLink;
    void (*sync) ();
};

struct RslRaster
{
    void    *commandBuffer;
    uint8_t *data;
    int16_t  stride;
    uint8_t  logWidth;
    uint8_t  logHeight;
    uint8_t  depth;

    uint8_t mipmaps : 6;
    bool    uClamp : 1;
    bool    vClamp : 1;

    uint8_t unk;

    enum ColorMode
    {
        GU_PSM_5551 = 1,
        GU_PSM_8888 = 5,
    } colorMode : 5;

    enum PaletteMode
    {
        NONE,
        GU_PSM_T8,
        GU_PSM_T4,


    } paletteMode : 3;

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

class RslNode : public RslObject<>
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

class RslGeometry : public RslObject<>
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

class RslElementGroup : public RslObject<RslNode>
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
    struct RslRaster              *raster;
    struct RslTexList             *dict;
    struct RslLLLink<RslTexture *> lLink;
    char                           name[32];
    char                           mask[32];
};

struct RslTexList : public RslObject<>
{
    RslLinkList<RslTexture *> textures;
    RslLLLink<RslTexList *>   lLink;
};
