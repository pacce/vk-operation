#ifndef VK_PIPELINE_HPP__
#define VK_PIPELINE_HPP__

#include <vulkan/vulkan.h>
#include <cstdint>

#include "vk-descriptor.hpp"
#include "vk-device.hpp"
#include "vk-shader.hpp"

namespace vk {
    struct PipelineLayout {
        public:
            VkPipelineLayout layout;

            PipelineLayout(vk::Device device, const vk::DescriptorSetLayout& set)
                : device_(device)
            {
                VkPipelineLayoutCreateInfo info{};
                info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                info.setLayoutCount = 1;
                info.pSetLayouts    = &(set.layout);

                layout = device_->create_pipeline_layout(info);
            }

            ~PipelineLayout() {
                device_->destroy_pipeline_layout(layout);
            }
        private:
            vk::Device device_;
    };

    struct Pipeline {
        public:
            VkPipeline pipeline;

            Pipeline(
                vk::Device              device
                , const vk::Shader&     module
                , vk::PipelineLayout&   layout
                )
                : device_(device)
            {
                VkComputePipelineCreateInfo info{};
                info.sType          = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
                info.stage.sType    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                info.stage.stage    = VK_SHADER_STAGE_COMPUTE_BIT;
                info.stage.module   = module.shader;
                info.stage.pName    = "main";
                info.layout         = layout.layout;

                pipeline = device_->create_compute_pipeline(info);
            }

            ~Pipeline() {
                device_->destroy_pipeline(pipeline);
            }
        private:
            vk::Device device_;
    };
} // namespace vk

#endif //  VK_PIPELINE_HPP__
