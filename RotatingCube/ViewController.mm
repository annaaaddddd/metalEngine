//
//  ViewController.m
//  myGameEngine
//
//  Created by Anna Dai on 2025/6/17.
//

#import "ViewController.h"
#import "RendererAdapter.h"


@implementation ViewController
{
    MTKView * _view;
    
    RendererAdapter * _pRendererAdapter;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    _view = (MTKView *)self.view;
    _view.device = MTLCreateSystemDefaultDevice();
    _view.depthStencilPixelFormat = MTLPixelFormatDepth32Float;
    
    _pRendererAdapter = [[RendererAdapter alloc]initWithMTKView:_view];
    _view.delegate = _pRendererAdapter;
    // _view.preferredFramesPerSecond = 30; // Set preferred fps here; default is 60
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
