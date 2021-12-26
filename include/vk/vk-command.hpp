#ifndef VK_COMMAND_HPP__
#define VK_COMMAND_HPP__

#include <vulkan/vulkan.h>
#include <cstdint>

#include "vk-descriptor.hpp"
#include "vk-device.hpp"
#include "vk-pipeline.hpp"

namespace vk {
    struct CommandPool {
        public:
            VkCommandPool pool;
            CommandPool(vk::Device& device, uint32_t family) : device_(device) {
                VkCommandPoolCreateInfo info{};
                info.sType              = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
                info.flags              = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
                info.queueFamilyIndex   = family;

                pool = device_->create_command_pool(info);
            }

            ~CommandPool() {
                device_->destroy_command_pool(pool);
            }
        private:
            vk::Device device_;
    };

    struct CommandBuffer {
        public:
            VkCommandBuffer buffer;
            CommandBuffer(vk::Device& device, const CommandPool& pool)
                : device_(device)
            {
                VkCommandBufferAllocateInfo info{};
                info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                info.commandPool        = pool.pool;
                info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                info.commandBufferCount = 1;

                buffer = device_->allocate_command_buffers(info);
            }

            void
            begin() {
                VkCommandBufferBeginInfo info{};
                info.sType  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                info.flags  = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

                vkBeginCommandBuffer(buffer, &info);
            }

            void
            end() {
                VkResult result = vkEndCommandBuffer(buffer);
                if (VK_SUCCESS != result) {
                    throw std::runtime_error("failed to end command buffer");
                }
            }

            void
            bind_pipeline(const vk::Pipeline& pipeline) {
                vkCmdBindPipeline(
                    buffer
                    , VK_PIPELINE_BIND_POINT_COMPUTE
                    , pipeline.pipeline
                    );
            }

            void
            bind_descriptor(
                    const vk::PipelineLayout&   layout
                    , const vk::DescriptorSet&  descriptor
                    )
            {
                vkCmdBindDescriptorSets(
                        buffer
                        , VK_PIPELINE_BIND_POINT_COMPUTE
                        , layout.layout
                        , 0
                        , 1
                        , &(descriptor.set)
                        , 0
                        , nullptr
                        );
            }
            void
            dispatch(std::size_t count) {
                vkCmdDispatch(buffer, static_cast<uint32_t>(count), 1, 1);
            }

            void
            dispatch(std::size_t width, std::size_t height) {
                vkCmdDispatch(
                        buffer
                        , static_cast<uint32_t>(width)
                        , static_cast<uint32_t>(height)
                        , 1
                        );
            }

            ~CommandBuffer() {}
        private:
            vk::Device device_;
    };
} // namespace vk

#endif // VK_COMMAND_HPP__
