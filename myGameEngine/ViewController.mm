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
    _pRendererAdapter = [RendererAdapter alloc];
    [_pRendererAdapter draw:_view.currentDrawable device:_view.device];
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
