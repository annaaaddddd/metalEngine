//
//  RendererAdapter.m
//  myGameEngine
//
//  Created by Anna Dai on 2025/6/20.
//

#import <Foundation/Foundation.h>
#import "RendererAdapter.h"

@implementation RendererAdapter
- (void)draw:(id <CAMetalDrawable>) drawable device: (id <MTLDevice>) device
{
    _pRenderer = new Renderer((__bridge CA::MetalDrawable *) drawable, (__bridge MTL::Device *) device);
    _pRenderer->draw();
}
-(void)dealloc
{
    delete _pRenderer;
}

@end
