#ifndef VK_DESCRIPTOR_HPP__
#define VK_DESCRIPTOR_HPP__

#include <memory>
#include <vulkan/vulkan.h>
#include <vector>

#include "vk-buffer.hpp"
#include "vk-device.hpp"

namespace vk {
    struct DescriptorSetLayout {
        public:
            VkDescriptorSetLayout layout;

            DescriptorSetLayout(vk::Device& device, std::size_t bindings)
                : device_(device)
            {
                std::vector<VkDescriptorSetLayoutBinding> layouts;
                for (std::size_t binding = 0; binding < bindings; binding++) {
                    VkDescriptorSetLayoutBinding layout{};
                    layout.binding          = static_cast<uint32_t>(binding);
                    layout.descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                    layout.descriptorCount  = 1;
                    layout.stageFlags       = VK_SHADER_STAGE_COMPUTE_BIT;
                    layouts.push_back(layout);
                }

                VkDescriptorSetLayoutCreateInfo info{};
                info.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                info.bindingCount   = static_cast<uint32_t>(layouts.size());
                info.pBindings      = layouts.data();

                layout = device_->create_descriptor_set_layout(info);
            }

            ~DescriptorSetLayout() {
                device_->destroy_descriptor_set_layout(layout);
            }
        private:
            vk::Device device_;
    };

    struct DescriptorPool {
        public:
            VkDescriptorPool pool;

            DescriptorPool(vk::Device& device, std::size_t descriptors) : device_(device) {
                VkDescriptorPoolSize size{};
                size.type               = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                size.descriptorCount    = static_cast<uint32_t>(descriptors);

                VkDescriptorPoolCreateInfo info{};
                info.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                info.maxSets        = 1;
                info.poolSizeCount  = 1;
                info.pPoolSizes     = &size;
                info.flags          = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

                pool = device_->create_descriptor_pool(info);
            }

            ~DescriptorPool() {
                device_->destroy_descriptor_pool(pool);
            }
        private:
            vk::Device device_;
    };

    struct DescriptorSet {
        public:
            VkDescriptorSet set;

            DescriptorSet(
                    vk::Device&                         device
                    , std::shared_ptr<DescriptorPool>   pool
                    , const DescriptorSetLayout&        layout
                    )
                : pool_(pool)
                , device_(device)
            {
                VkDescriptorSetAllocateInfo info{};

                info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                info.descriptorPool     = pool_->pool;
                info.descriptorSetCount = 1;
                info.pSetLayouts        = &(layout.layout);

                set = device_->allocate_descriptor_sets(info);
            }

            ~DescriptorSet() {
                device_->free_descriptor_sets(pool_->pool, set);
            }

            template <typename T>
            void
            update(const vk::Buffer<T>& buffers) {
                std::vector<VkWriteDescriptorSet> xs(buffers.size());
                std::vector<VkDescriptorBufferInfo> infos(buffers.size());

                uint32_t i = 0;
                for (const VkBuffer& buffer : buffers.buffers) {
                    VkDescriptorBufferInfo info{};
                    info.buffer = buffer;
                    info.offset = 0;
                    info.range  = VK_WHOLE_SIZE;
                    infos[i]    = info;

                    VkWriteDescriptorSet x{};
                    x.sType             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    x.dstSet            = set;
                    x.dstBinding        = i;
                    x.dstArrayElement   = 0;
                    x.descriptorCount   = 1;
                    x.descriptorType    = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                    x.pBufferInfo       = &infos[i];
                    xs[i] = x;

                    i++;
                }
                device_->update_descriptor_sets(xs);
            }
        private:
            std::shared_ptr<DescriptorPool> pool_;
            vk::Device                      device_;
    };
} // namespace vk

#endif // VK_DESCRIPTOR_HPP__
