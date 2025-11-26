#pragma once
#include "hk/gfx/DebugRenderer.h"
class ExtendedRenderer {
    public:
        ExtendedRenderer(hk::gfx::DebugRenderer* renderer) : mRenderer(renderer) {}
        hk::gfx::DebugRenderer* mRenderer;
        void drawRect(f32 x, f32 y, f32 width, f32 height, u32 color, f32 round = 0.0f) {
            mRenderer->drawQuad(
                { { x, y }, { 0, 0 }, color },
                { { x + width, y }, { 1.0, 0 }, color },
                { { x + width, y + height }, { 1.0, 1.0 }, color },
                { { x, y + height }, { 0, 1.0 }, color },
                round);
        }
        template<size lineLength>
        void printWrap(const char* text) {
            const size tlen = strlen(text);
            for (int line_start = 0; line_start < tlen;) {
                const size chars_left = tlen - line_start;
                size new_line_length = lineLength;
                // there is more than a full line left to draw, find the last space and cut there
                if (chars_left > lineLength) {
                    do {
                        new_line_length--;
                    } while (new_line_length > 0 && text[line_start + new_line_length] != ' ');
                    // no breaks, present the whole line
                    if (new_line_length <= 0) {
                        new_line_length = lineLength;
                    }
                }
                char line[lineLength + 3];
                memset(line, 0, lineLength + 3);
                strncpy(line, text + line_start, new_line_length);
                line[new_line_length] = '\r';
                line[new_line_length + 1] = '\n';
                mRenderer->printf(line);
                line_start += new_line_length + 1;
            }
        }
};