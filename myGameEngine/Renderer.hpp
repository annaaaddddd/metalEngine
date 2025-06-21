//
//  Renderer.hpp
//  myGameEngine
//
//  Created by Anna Dai on 2025/6/17.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"

class Renderer {
public:
    Renderer(CA::MetalDrawable * const pDrawable, MTL::Device * const pDevice); // Drawable - canvas; device - gpu
    ~Renderer();
    void draw() const;
    
private:
    CA::MetalDrawable * _pDrawable;
    MTL::Device * const _pDevice;
    MTL::CommandQueue * const _pCommandQueue;
};

#endif /* Renderer_hpp */
