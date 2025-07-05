//
//  Shaders.metal
//  vertex and fragment shader for triangle in myGameEngine
//
//  Created by Anna Dai on 2025/7/1.
//

#include <metal_stdlib>
using namespace metal;

// Transform each vertex according to the given location
// The reason to use packed_float3 instead of float is to avoid the memory alignment issue that happens when
// when Metal can only start storing a new memory at address divisible by 16. (this way the vertices will be stored continuously)
[[vertex]] float4 vertexShader(const constant packed_float3 * triangleVertices [[buffer(5)]],
                               const constant float * timer [[buffer(7)]],
                               const ushort vertexIndex [[vertex_id]])
{
    const float x = triangleVertices[vertexIndex].x + sin(*timer);
    const float y = triangleVertices[vertexIndex].y;
    const float z = triangleVertices[vertexIndex].z;
    return float4(x, y, z, 1.0f);
}
 
// Assign pixel; rasterization - mapping 3d to 2d, using z buffer
[[fragment]] float4 fragmentShader() {
    return float4(1.0f, .0f, 1.0f, 1.0f);
}
