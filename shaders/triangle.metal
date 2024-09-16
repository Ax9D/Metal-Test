#include <metal_stdlib>
using namespace metal;

// constant float3 triangleVerts[] = {
//     float3( 1.0, -1.0, 0.0),
//     float3( 0.0,  1.0, 0.0), 
//     float3(-1.0, -1.0, 0.0),
// };

struct GlobalUBO {
    float3 color;
};

struct PerMeshUBO {
    float4x4 transform;
};

struct VertexIn {
    float3 pos [[attribute(0)]];
    float3 vert [[attribute(1)]];
    float2 uv [[attribute(2)]];
};

vertex float4 vertexShader(VertexIn vertexIn [[stage_in]], constant PerMeshUBO& mesh_data[[buffer(1)]]) {
    float4 outputVertex = mesh_data.transform * float4(vertexIn.pos, 1.0);

    return outputVertex;
}

fragment float4 fragmentShader(float4 vertexPosition [[stage_in]], constant GlobalUBO& ubo_data[[buffer(0)]]) {
    return float4(ubo_data.color, 1.0);
}