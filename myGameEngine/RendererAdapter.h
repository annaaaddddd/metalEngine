//
//  Header.h
//  myGameEngine
//
//  Created by Anna Dai on 2025/6/20.
//


#import "QuartzCore/CAMetalLayer.h"
#import "Metal/MTLDevice.h"
#import "Renderer.hpp"

@interface RendererAdapter : NSObject
{
    Renderer * _pRenderer;
}

-(void)draw:(id <CAMetalDrawable>) drawable device: (id <MTLDevice>) device;

@end
