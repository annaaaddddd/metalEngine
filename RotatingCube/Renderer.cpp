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

static const std::vector<float> cubeVertices() {
  return std::vector<float> {
    // front red - vertices 0-3
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 0: top left
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 1: bottom left
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 2: bottom right
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 3: top right
    
    // back green - vertices 4-7
    -0.5f,  0.5f, 1.0f,  0.0f, 1.0f, 0.0f,  // 4: top left
    -0.5f, -0.5f, 1.0f,  0.0f, 1.0f, 0.0f,  // 5: bottom left
     0.5f, -0.5f, 1.0f,  0.0f, 1.0f, 0.0f,  // 6: bottom right
     0.5f,  0.5f, 1.0f,  0.0f, 1.0f, 0.0f,  // 7: top right
    
    // left blue - vertices 8-11
    -0.5f,  0.5f, 1.0f,  0.0f, 0.0f, 1.0f,  // 8: back top
    -0.5f, -0.5f, 1.0f,  0.0f, 0.0f, 1.0f,  // 9: back bottom
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // 10: front bottom
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // 11: front top
    
    // right orange - vertices 12-15
     0.5f,  0.5f, 0.0f,  1.0f, 0.4f, 0.0f,  // 12: front top
     0.5f, -0.5f, 0.0f,  1.0f, 0.4f, 0.0f,  // 13: front bottom
     0.5f, -0.5f, 1.0f,  1.0f, 0.4f, 0.0f,  // 14: back bottom
     0.5f,  0.5f, 1.0f,  1.0f, 0.4f, 0.0f,  // 15: back top
    
    // top purple - vertices 16-19
    -0.5f,  0.5f, 1.0f,  1.0f, 0.0f, 1.0f,  // 16: back left
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f,  // 17: front left
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f,  // 18: front right
     0.5f,  0.5f, 1.0f,  1.0f, 0.0f, 1.0f,  // 19: back right
    
    // bottom cyan - vertices 20-23
    -0.5f, -0.5f, 1.0f,  0.0f, 1.0f, 1.0f,  // 20: back left
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,  // 21: front left
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,  // 22: front right
     0.5f, -0.5f, 1.0f,  0.0f, 1.0f, 1.0f,  // 23: back right
  };
}

Renderer:: Renderer (MTL::Device * const pDevice)
:
  _pDevice(pDevice),
  _pCommandQueue (_pDevice->newCommandQueue()),
  _pRenderPipelineState(nullptr, [](MTL::RenderPipelineState * const p) { p->release(); }),
  _angle()
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
    pRpd->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);

    NS::Error * pErr {nullptr};
    _pRenderPipelineState.reset(_pDevice->newRenderPipelineState(pRpd.get(), &pErr));
    if (!_pRenderPipelineState.get()) {
        __builtin_printf("%s", pErr->localizedDescription()->utf8String());
    }
}


void Renderer::drawFrame(const CA::MetalDrawable *const pDrawable, const MTL::Texture * const pDepthTexture)
{
    MTL::CommandBuffer * pCmdBuf = _pCommandQueue->commandBuffer();
    
    // Setting screen background to green
    MTL::RenderPassDescriptor * pRpd = MTL::RenderPassDescriptor::alloc()->init();
    pRpd->colorAttachments()->object(0)->setTexture(pDrawable->texture());
    pRpd->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    pRpd->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0, 1.0, 0.0, 1.0));
    
    const std::unique_ptr<MTL::RenderPassDepthAttachmentDescriptor, void(*)(MTL::RenderPassDepthAttachmentDescriptor * const)> pRenderPassDepthAttachmentDescriptor(MTL::RenderPassDepthAttachmentDescriptor::alloc()->init(), [](MTL::RenderPassDepthAttachmentDescriptor * const d) { d->release(); });
    pRenderPassDepthAttachmentDescriptor->setTexture(pDepthTexture);
    
    pRpd->setDepthAttachment(pRenderPassDepthAttachmentDescriptor.get());
    
    const std::unique_ptr<MTL::DepthStencilDescriptor, void(*)(MTL::DepthStencilDescriptor * const)> pDepthStencilDescriptor(MTL::DepthStencilDescriptor::alloc()->init(), [](MTL::DepthStencilDescriptor * const d) { d->release(); });
    pDepthStencilDescriptor->setDepthWriteEnabled(true);
    pDepthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionLess);
    
    const std::unique_ptr<MTL::DepthStencilState, void(*)(MTL::DepthStencilState * const)> pDepthStencilState(_pDevice->newDepthStencilState(pDepthStencilDescriptor.get()), [](MTL::DepthStencilState * const d) { d->release(); });
    
    
    const std::vector<float> vertices = cubeVertices();
    
    const std::vector<ushort> indices = {
        0, 1, 2,  2, 3, 0, // front red
        4, 6, 5,  6, 4, 7, // back green
        8, 9, 10,  10, 11, 8, // left blue
        12, 13, 14,  14, 15, 12, // right orange
        16, 17, 18,  18, 19, 16, // top purple
        20, 22, 21,  22, 20, 23 // bottom cyan
    };
    
    const std::unique_ptr<MTL::Buffer, void(*) (MTL::Buffer * const)> pVertexBuffer(_pDevice->newBuffer(vertices.data(),sizeof(float) * vertices.size(), MTL::ResourceStorageModeShared), [](MTL::Buffer * const b) { b->release(); });
    
    const std::unique_ptr<MTL::Buffer, void(*) (MTL::Buffer * const)> pIndexBuffer(_pDevice->newBuffer(indices.data(),sizeof(ushort) * indices.size(), MTL::ResourceStorageModeShared), [](MTL::Buffer * const b) { b->release(); });
    
    
    _angle += 0.01f;
    
    MTL::RenderCommandEncoder * pEnc = pCmdBuf->renderCommandEncoder(pRpd);
    pEnc->setDepthStencilState(pDepthStencilState.get());
    pEnc->setRenderPipelineState(_pRenderPipelineState.get());
    pEnc->setVertexBuffer(pVertexBuffer.get(), 0, 5);
    pEnc->setVertexBytes(&_angle, sizeof(float), 7); // Lightweight alternative to set VertexBuffer; Recommend when passing data less than 4KB
    
    pEnc->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, indices.size(), MTL::IndexTypeUInt16, pIndexBuffer.get(), 0);
    
    pEnc->endEncoding();
    pCmdBuf->presentDrawable(pDrawable);
    pCmdBuf->commit();
    
    pRpd->release();
}
