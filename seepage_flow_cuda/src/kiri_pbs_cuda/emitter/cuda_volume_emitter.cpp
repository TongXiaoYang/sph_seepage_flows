/***
 * @Author: Xu.WANG raymondmgwx@gmail.com
 * @Date: 2023-03-21 12:32:23
 * @LastEditors: Xu.WANG raymondmgwx@gmail.com
 * @LastEditTime: 2023-03-21 18:54:02
 * @FilePath:
 * \sph_seepage_flows\seepage_flow_cuda\src\kiri_pbs_cuda\emitter\cuda_volume_emitter.cpp
 * @Description:
 * @Copyright (c) 2023 by Xu.WANG, All Rights Reserved.
 */
#include <kiri_pbs_cuda/emitter/cuda_volume_emitter.cuh>
#include <random>
namespace KIRI {

void CudaVolumeEmitter::BuildSeepageflowShapeVolume(
    SeepageflowVolumeData &data, Vec_Float4 shape, float3 color,
    float sandDensity, bool offsetY, float worldLowestY, float2 offsetXZ) {
  if (!bEnable)
    return;

  data.sandMinRadius = Huge<size_t>();
  float minY = Huge<size_t>();

  for (size_t i = 0; i < shape.size(); i++) {
    float radius = shape[i].w;
    float mass = sandDensity * ((4.f / 3.f) * KIRI_PI * std::powf(radius, 3.f));

    data.pos.emplace_back(make_float3(shape[i].x + offsetXZ.x, shape[i].y,
                                      shape[i].z + offsetXZ.y));
    data.col.emplace_back(color);
    data.label.emplace_back(1);
    data.radius.emplace_back(radius);
    data.mass.emplace_back(mass);
    data.inertia.emplace_back(2.f / 5.f * mass * radius * radius);
    data.sandMinRadius = std::min(radius, data.sandMinRadius);
    minY = std::min(minY, shape[i].y);
  }

  if (offsetY) {
    float offsetYVal = minY - (worldLowestY + data.sandMinRadius * 5.f);
    for (size_t i = 0; i < data.pos.size(); i++)
      data.pos[i] -= make_float3(0.f, offsetYVal, 0.f);
  }
}

void CudaVolumeEmitter::BuildSeepageflowShapeMultiVolume(
    SeepageflowMultiVolumeData &data, Vec_Float4 shape, float3 color,
    float sandDensity, float3 cda0asat, float2 amcamcp, bool offsetY,
    float worldLowestY, float2 offsetXZ) {
  if (!bEnable)
    return;

  if (data.pos.size() == 0)
    data.sandMinRadius = Huge<size_t>();

  float minY = Huge<size_t>();

  for (size_t i = 0; i < shape.size(); i++) {
    float radius = shape[i].w;
    float mass = sandDensity * ((4.f / 3.f) * KIRI_PI * std::powf(radius, 3.f));
    data.pos.emplace_back(make_float3(shape[i].x + offsetXZ.x, shape[i].y,
                                      shape[i].z + offsetXZ.y));
    data.col.emplace_back(color);
    data.label.emplace_back(1);
    data.radius.emplace_back(radius);
    data.mass.emplace_back(mass);
    data.inertia.emplace_back(2.f / 5.f * mass * radius * radius);
    data.sandMinRadius = std::min(radius, data.sandMinRadius);
    data.cda0asat.emplace_back(make_float3(cda0asat));
    data.amcamcp.emplace_back(make_float2(amcamcp));
    minY = std::min(minY, shape[i].y);
  }

  if (offsetY) {
    float offsetYVal = minY - (worldLowestY + data.sandMinRadius * 5.f);
    for (size_t i = 0; i < data.pos.size(); i++)
      data.pos[i] -= make_float3(0.f, offsetYVal, 0.f);
  }
}
} // namespace KIRI
