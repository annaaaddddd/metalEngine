//
//  Renderer.cpp
//  myGameEngine
//
//  Created by Anna Dai on 2025/6/17.
//

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

//#include <Foundation/Foundation.hpp>
//#include <Metal/Metal.hpp>
//#include <QuartzCore/QuartzCore.hpp>

#include "Renderer.hpp"
#include <iostream>
#include <vector>

Renderer:: Renderer (MTL::Device * const pDevice)
:
  _pDevice(pDevice),
  _pCommandQueue (_pDevice->newCommandQueue()),
  _pRenderPipelineState(nullptr, [](MTL::RenderPipelineState * const p) { p->release(); }),
  _timer()
{
    buildShaders();
}

Renderer:: ~Renderer()
{
    _pCommandQueue->release();
}

void Renderer::buildShaders()
{
    // MTL Library can join all shader functions into one file
    const std::unique_ptr<MTL::Library, void(*)(MTL::Library * const)> pLib(_pDevice->newDefaultLibrary(), [](MTL::Library * const l) { l->release(); });
    
    // Acquire the two custom functions for triangle
    const std::unique_ptr<MTL::Function, void(*)(MTL::Function * const)> pVertexFn(pLib.get()->newFunction(NS::String::string("vertexShader", NS::UTF8StringEncoding)), [](MTL::Function * const f) { f->release(); });
    const std::unique_ptr<MTL::Function, void(*)(MTL::Function * const)> pFragmentFn(pLib.get()->newFunction(NS::String::string("fragmentShader", NS::UTF8StringEncoding)), [](MTL::Function * const f) { f->release(); });

    const std::unique_ptr<MTL::RenderPipelineDescriptor, void(*)(MTL::RenderPipelineDescriptor * const)> pRpd(MTL::RenderPipelineDescriptor::alloc()->init(), [](MTL::RenderPipelineDescriptor * const d) { d->release(); });
    pRpd.get()->setVertexFunction(pVertexFn.get());
    pRpd.get()->setFragmentFunction(pFragmentFn.get());
    pRpd.get()->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

    NS::Error * pErr {nullptr};
    _pRenderPipelineState.reset(_pDevice->newRenderPipelineState(pRpd.get(), &pErr));
    if (!_pRenderPipelineState.get()) {
    __builtin_printf("%s", pErr->localizedDescription()->utf8String());
    }
}


void Renderer::drawFrame(const CA::MetalDrawable *const pDrawable)
{
    MTL::CommandBuffer * pCmdBuf = _pCommandQueue->commandBuffer();
    
    // Setting screen background to green
    MTL::RenderPassDescriptor * pRpd = MTL::RenderPassDescriptor::alloc()->init();
    pRpd->colorAttachments()->object(0)->setTexture(pDrawable->texture());
    pRpd->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    pRpd->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0, 1.0, 0.0, 1.0));
    
    // Adding vertices of a triangle
    const std::vector<float> triangle = {
        -0.5f, 0.5f, .0f,
        0.5f, 0.5f, .0f,
        .0f, -1.0f, .0f
    };
    
    const std::unique_ptr<MTL::Buffer, void(*) (MTL::Buffer * const)> pVertexBuffer(_pDevice->newBuffer(triangle.data(),sizeof(float) * 9, MTL::ResourceStorageModeShared), [](MTL::Buffer * const b) { b->release(); }); // costum deleter for the allocated mem for this triangle
    
    _timer += 0.01;
    
    MTL::RenderCommandEncoder * pEnc = pCmdBuf->renderCommandEncoder(pRpd);
    pEnc->setRenderPipelineState(_pRenderPipelineState.get());
    pEnc->setVertexBuffer(pVertexBuffer.get(), 0, 5);
    pEnc->setVertexBytes(&_timer, sizeof(float), 7); // Lightweight alternative to set VertexBuffer; Recommend when passing data less than 4KB
    pEnc->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    pEnc->endEncoding();
    pCmdBuf->presentDrawable(pDrawable);
    pCmdBuf->commit();
    
    pRpd->release();
}
