#include "Backend.hh"
#include <cstdint>
#include "core/Logger.hh"
#include "debug/imgui/Interop.hh"
#include "pspgu.h"

namespace ImGuiPsp {
void
swizzle_fast (u8 *out, const u8 *in, unsigned int width, unsigned int height)
{
    unsigned int blockx, blocky;
    unsigned int j;

    unsigned int width_blocks  = (width / 16);
    unsigned int height_blocks = (height / 8);

    unsigned int src_pitch = (width - 16) / 4;
    unsigned int src_row   = width * 8;

    const u8 *ysrc = in;
    u32      *dst  = (u32 *) out;

    for (blocky = 0; blocky < height_blocks; ++blocky)
        {
            const u8 *xsrc = ysrc;
            for (blockx = 0; blockx < width_blocks; ++blockx)
                {
                    const u32 *src = (u32 *) xsrc;
                    for (j = 0; j < 8; ++j)
                        {
                            *(dst++) = *(src++);
                            *(dst++) = *(src++);
                            *(dst++) = *(src++);
                            *(dst++) = *(src++);
                            src += src_pitch;
                        }
                    xsrc += 16;
                }
            ysrc += src_row;
        }
}

struct Texture
{
    unsigned char *pixels;
    int            width, height;
    int            flags;

    Texture (int width, int height, unsigned char *pixels, int flags)
        : width (width), height (height), pixels (pixels), flags (flags)
    {
        uint32_t *swizzled = new uint32_t[width * height];
        memcpy (swizzled, pixels, width * height * 4);
        swizzle_fast ((unsigned char *) swizzled, pixels, width * 4, height);
        this->pixels = (unsigned char *) swizzled;
        delete[] pixels;
    }
};

struct Vertex
{
    float    u, v;
    uint32_t color;
    float    x, y, z;
};

static Vertex vertex_buffer[100'000];
static int    vertex_buffer_index = 0;

void
DrawImguiCommand (const ImDrawCmd *pcmd, const ImDrawList *cmd_list)
{
    const ImDrawVert *vtx_buffer = cmd_list->VtxBuffer.Data;
    const ImDrawIdx  *idx_buffer = cmd_list->IdxBuffer.Data;

    float Scale = GetDrawingScale ();

    // Init
    sceGuDisable (GU_DEPTH_TEST);

    Texture *tex = (Texture *) (pcmd->GetTexID ());

    if (tex)
        {
            sceGuEnable (GU_TEXTURE_2D);
            sceGuTexMode (tex->flags, 0, 0, 1);
            sceGuTexMapMode (GU_TEXTURE_COORDS, 0, 0);
            sceGuTexImage (0, tex->width, tex->height, tex->width, tex->pixels);
        }
    else
        sceGuDisable (GU_TEXTURE_2D);

    // Scissoring
    sceGuScissor (pcmd->ClipRect.x / Scale, pcmd->ClipRect.y / Scale,
                  pcmd->ClipRect.z / Scale, pcmd->ClipRect.w / Scale);
    sceGuEnable (GU_SCISSOR_TEST);

    sceGuDrawArray (GU_TRIANGLES,
                    GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TEXTURE_32BITF
                        | GU_TRANSFORM_2D | GU_INDEX_16BIT,
                    pcmd->ElemCount, idx_buffer + pcmd->IdxOffset,
                    vertex_buffer + pcmd->VtxOffset + vertex_buffer_index);
    // Clean
    sceGuDisable (GU_DEPTH_TEST);
}

void
ConvertVertices (const ImDrawVert *vtx_buffer, size_t count)
{
    float Scale = GetDrawingScale ();

    for (size_t i = 0; i < count; i++)
        {
            const ImDrawVert &v = vtx_buffer[i];
            vertex_buffer[vertex_buffer_index + i].color = v.col;
            vertex_buffer[vertex_buffer_index + i].x     = v.pos.x / Scale;
            vertex_buffer[vertex_buffer_index + i].y     = v.pos.y / Scale;
            vertex_buffer[vertex_buffer_index + i].z     = 0;
            vertex_buffer[vertex_buffer_index + i].u     = v.uv.x * 512;
            vertex_buffer[vertex_buffer_index + i].v     = v.uv.y * 64;
        }
}

void
RenderDrawData (ImDrawData *data)
{
    vertex_buffer_index = 0;
    for (size_t i = 0; i < data->CmdListsCount; i++)
        {
            const ImDrawList *cmd_list   = data->CmdLists[i];
            const ImDrawVert *vtx_buffer = cmd_list->VtxBuffer.Data;
            const ImDrawIdx  *idx_buffer = cmd_list->IdxBuffer.Data;

            ConvertVertices (vtx_buffer, cmd_list->VtxBuffer.Size);

            for (size_t j = 0; j < cmd_list->CmdBuffer.Size; j++)
                {
                    const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[j];
                    if (pcmd->UserCallback)
                        {
                            pcmd->UserCallback (cmd_list, pcmd);
                            return;
                        }

                    GuCommandWrapper ([pcmd, cmd_list] {
                        DrawImguiCommand (pcmd, cmd_list);
                    });
                }

            vertex_buffer_index += cmd_list->VtxBuffer.Size;
        }
}

void
Initialise ()
{
    ImGuiIO &io            = ImGui::GetIO ();
    io.BackendRendererName = "imgui_impl_psp";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    io.BackendPlatformName = "PSP";
    io.DisplaySize.x       = 512 * GetDrawingScale ();
    io.DisplaySize.y       = 320 * GetDrawingScale ();

    io.MouseDrawCursor = true;

    // initial mouse position
    io.AddMousePosEvent (200, 200);
    io.AddMouseSourceEvent (ImGuiMouseSource_Mouse);

    CreateFontsTexture ();
}

float &
GetDrawingScale ()
{
    static float sm_DrawingScale = 2.5f;
    return sm_DrawingScale;
}

void
CreateFontsTexture ()
{
    ImGuiIO       &io = ImGui::GetIO ();
    unsigned char *pixels;
    int            width, height;

    io.Fonts->GetTexDataAsRGBA32 (&pixels, &width, &height);

    Texture *texture = new Texture (width, height, pixels, GU_PSM_8888);
    io.Fonts->SetTexID ((ImTextureID) texture);
}
}; // namespace ImGuiPsp
