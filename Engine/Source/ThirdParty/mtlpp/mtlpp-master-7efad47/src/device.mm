/*
 * Copyright 2016-2017 Nikolay Aleksiev. All rights reserved.
 * License: https://github.com/naleksiev/mtlpp/blob/master/LICENSE
 */
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Modifications for Unreal Engine

#include <Metal/MTLDevice.h>
#include "device.hpp"
#include "buffer.hpp"
#include "command_queue.hpp"
#include "compute_pipeline.hpp"
#include "depth_stencil.hpp"
#include "render_pipeline.hpp"
#include "sampler.hpp"
#include "texture.hpp"
#include "heap.hpp"
#include "argument_encoder.hpp"

MTLPP_BEGIN

namespace mtlpp
{
	ArgumentDescriptor::ArgumentDescriptor()
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
	: ns::Object<MTLArgumentDescriptor*>([MTLArgumentDescriptor new], false)
#endif
	{
	}
	
	DataType ArgumentDescriptor::GetDataType() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return (DataType)[(MTLArgumentDescriptor*)m_ptr dataType];
#else
		return 0;
#endif
	}
	
	NSUInteger ArgumentDescriptor::GetIndex() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return [(MTLArgumentDescriptor*)m_ptr index];
#else
		return 0;
#endif
	}
	
	NSUInteger ArgumentDescriptor::GetArrayLength() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return [(MTLArgumentDescriptor*)m_ptr arrayLength];
#else
		return 0;
#endif
	}
	
	ArgumentAccess ArgumentDescriptor::GetAccess() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return (ArgumentAccess)[(MTLArgumentDescriptor*)m_ptr access];
#else
		return 0;
#endif
	}
	
	TextureType ArgumentDescriptor::GetTextureType() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return (TextureType)[(MTLArgumentDescriptor*)m_ptr textureType];
#else
		return 0;
#endif
	}
	
	NSUInteger ArgumentDescriptor::GetConstantBlockAlignment() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return [(MTLArgumentDescriptor*)m_ptr constantBlockAlignment];
#else
		return 0;
#endif
	}
	
    CompileOptions::CompileOptions() :
        ns::Object<MTLCompileOptions*>([[MTLCompileOptions alloc] init], false)
    {
    }

	ns::String Device::GetWasAddedNotification()
	{
#if MTLPP_IS_AVAILABLE_MAC(10_13)
		return MTLDeviceWasAddedNotification;
#else
		return ns::String();
#endif
	}
	
	ns::String Device::GetRemovalRequestedNotification()
	{
#if MTLPP_IS_AVAILABLE_MAC(10_13)
		return MTLDeviceRemovalRequestedNotification;
#else
		return ns::String();
#endif
	}
	
	ns::String Device::GetWasRemovedNotification()
	{
#if MTLPP_IS_AVAILABLE_MAC(10_13)
		return MTLDeviceWasRemovedNotification;
#else
		return ns::String();
#endif
	}
	
	ns::Array<ns::Ref<Device>> Device::CopyAllDevicesWithObserver(ns::Object<id <NSObject>> observer, DeviceHandler handler)
	{
#if MTLPP_IS_AVAILABLE_MAC(10_13)
		id<NSObject> obj = (id<NSObject>)observer.GetPtr();
		return MTLCopyAllDevicesWithObserver((id<NSObject>*)(obj ? &obj : nil), ^(id<MTLDevice>  _Nonnull device, MTLDeviceNotificationName  _Nonnull notifyName)
			{
			handler(ns::Ref<Device>(new Device(device)), ns::String(notifyName));
		});
#else
		return ns::Array<ns::Ref<Device>>();
#endif
	}
	
	void Device::RemoveDeviceObserver(ns::Object<id <NSObject>> observer)
	{
#if MTLPP_IS_AVAILABLE_MAC(10_13)
		if (observer)
		{
			MTLRemoveDeviceObserver((id<NSObject>)observer.GetPtr());
		}
#endif
	}
	
    ns::Ref<Device> Device::CreateSystemDefaultDevice()
    {
        return new Device(MTLCreateSystemDefaultDevice(), false);
    }

    ns::Array<ns::Ref<Device>> Device::CopyAllDevices()
    {
#if MTLPP_IS_AVAILABLE_MAC(10_11)
        return ns::Array<ns::Ref<Device>>(MTLCopyAllDevices(), false);
#else
		return [NSArray arrayWithObject:[MTLCreateSystemDefaultDevice() autorelease]];
#endif
    }

    ns::String Device::GetName() const
    {
        Validate();
		return m_table->name(m_ptr);
    }

    Size Device::GetMaxThreadsPerThreadgroup() const
    {
        Validate();
#if MTLPP_IS_AVAILABLE(10_11, 9_0)
		MTLPPSize MTLPPSize =  m_table->maxThreadsPerThreadgroup(m_ptr);
        return Size(NSUInteger(MTLPPSize.width), NSUInteger(MTLPPSize.height), NSUInteger(MTLPPSize.depth));
#else
        return Size(0, 0, 0);
#endif
    }

    bool Device::IsLowPower() const
    {
        Validate();
#if MTLPP_IS_AVAILABLE_MAC(10_11)
		return m_table->isLowPower(m_ptr);
#else
        return false;
#endif
    }

    bool Device::IsHeadless() const
    {
        Validate();
#if MTLPP_IS_AVAILABLE_MAC(10_11)
		return m_table->isHeadless(m_ptr);
#else
        return false;
#endif
    }
	
	bool Device::IsRemovable() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE_MAC(10_13)
		return m_table->isRemovable(m_ptr);
#else
		return false;
#endif
	}

    uint64_t Device::GetRecommendedMaxWorkingSetSize() const
    {
#if MTLPP_IS_AVAILABLE_MAC(10_12)
		return m_table->recommendedMaxWorkingSetSize(m_ptr);
#else
		return 0;
#endif
    }

    bool Device::IsDepth24Stencil8PixelFormatSupported() const
    {
        Validate();
#if MTLPP_IS_AVAILABLE_MAC(10_11)
		return m_table->isDepth24Stencil8PixelFormatSupported(m_ptr);
#else
        return true;
#endif
    }
	
	uint64_t Device::GetRegistryID() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return m_table->registryID(m_ptr);
#else
		return 0;
#endif
	}
	
	ReadWriteTextureTier Device::GetReadWriteTextureSupport() const
	{
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return (ReadWriteTextureTier)m_table->readWriteTextureSupport(m_ptr);
#else
		return 0;
#endif
	}
	
	ArgumentBuffersTier Device::GetArgumentsBufferSupport() const
	{
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return (ArgumentBuffersTier)m_table->argumentBuffersSupport(m_ptr);
#else
		return 0;
#endif
	}
	
	bool Device::AreRasterOrderGroupsSupported() const
	{
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return m_table->areRasterOrderGroupsSupported(m_ptr);
#else
		return false;
#endif
	}
	
	uint64_t Device::GetCurrentAllocatedSize() const
	{
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return m_table->currentAllocatedSize(m_ptr);
#else
		return 0;
#endif
	}

    CommandQueue Device::NewCommandQueue()
    {
        Validate();
		return m_table->NewCommandQueue(m_ptr);
    }

    CommandQueue Device::NewCommandQueue(NSUInteger maxCommandBufferCount)
    {
        Validate();
		return m_table->NewCommandQueueWithMaxCommandBufferCount(m_ptr, maxCommandBufferCount);
    }

    SizeAndAlign Device::HeapTextureSizeAndAlign(const TextureDescriptor& desc)
    {
#if MTLPP_IS_AVAILABLE(10_13, 10_0)
		MTLPPSizeAndAlign MTLPPSizeAndAlign = m_table->heapTextureSizeAndAlignWithDescriptor(m_ptr, desc.GetPtr());
		return SizeAndAlign{ NSUInteger(MTLPPSizeAndAlign.size), NSUInteger(MTLPPSizeAndAlign.align) };
#else
        return SizeAndAlign{0, 0};
#endif
    }

    SizeAndAlign Device::HeapBufferSizeAndAlign(NSUInteger length, ResourceOptions options)
    {
#if MTLPP_IS_AVAILABLE(10_13, 10_0)
		MTLPPSizeAndAlign MTLPPSizeAndAlign = m_table->heapBufferSizeAndAlignWithLengthoptions(m_ptr, length, MTLResourceOptions(options));
		return SizeAndAlign{ NSUInteger(MTLPPSizeAndAlign.size), NSUInteger(MTLPPSizeAndAlign.align) };
#else
        return SizeAndAlign{0, 0};
#endif
    }

    Heap Device::NewHeap(const HeapDescriptor& descriptor)
    {
#if MTLPP_IS_AVAILABLE(10_13, 10_0)
		return m_table->NewHeapWithDescriptor(m_ptr, descriptor.GetPtr());
#else
		return nullptr;
#endif
    }

    Buffer Device::NewBuffer(NSUInteger length, ResourceOptions options)
    {
        Validate();
		return m_table->NewBufferWithLength(m_ptr, length, MTLResourceOptions(options));
    }

    Buffer Device::NewBuffer(const void* pointer, NSUInteger length, ResourceOptions options)
    {
        Validate();
		return m_table->NewBufferWithBytes(m_ptr, pointer, length, MTLResourceOptions(options));
    }


    Buffer Device::NewBuffer(void* pointer, NSUInteger length, ResourceOptions options, BufferDeallocHandler deallocator)
    {
        Validate();
		return m_table->NewBufferWithBytesNoCopy(m_ptr, pointer, length, MTLResourceOptions(options), ^(void* pointer, NSUInteger length) { deallocator(pointer, NSUInteger(length)); });
    }

    DepthStencilState Device::NewDepthStencilState(const DepthStencilDescriptor& descriptor)
    {
        Validate();
		return m_table->NewDepthStencilStateWithDescriptor(m_ptr, descriptor.GetPtr());
    }

    Texture Device::NewTexture(const TextureDescriptor& descriptor)
    {
        Validate();
		return m_table->NewTextureWithDescriptor(m_ptr, descriptor.GetPtr());
    }
	
	Texture Device::NewTextureWithDescriptor(const TextureDescriptor& descriptor, ns::IOSurface& iosurface, NSUInteger plane)
	{
		Validate();
#if MTLPP_IS_AVAILABLE_MAC(10_11)
	return 	m_table->NewTextureWithDescriptorIOSurface(m_ptr, descriptor.GetPtr(), iosurface.GetPtr(), plane);
#else
		return Texture();
#endif
	}

    SamplerState Device::NewSamplerState(const SamplerDescriptor& descriptor)
    {
        Validate();
		return m_table->NewSamplerStateWithDescriptor(m_ptr, descriptor.GetPtr());
    }

    Library Device::NewDefaultLibrary()
    {
        Validate();
		return m_table->NewDefaultLibrary(m_ptr);
    }

    Library Device::NewLibrary(const ns::String& filepath, ns::AutoReleasedError* error)
    {
        Validate();
		return m_table->NewLibraryWithFile(m_ptr, filepath.GetPtr(), error ? (NSError**)error->GetInnerPtr() : nullptr);
    }
	
	Library Device::NewDefaultLibraryWithBundle(const ns::Bundle& bundle, ns::AutoReleasedError* error)
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_12, 10_0)
		return m_table->NewDefaultLibraryWithBundle(m_ptr, bundle.GetPtr(), error ? (NSError**)error->GetInnerPtr() : nullptr);
#else
		return Library();
#endif
	}

    Library Device::NewLibrary(const char* source, const CompileOptions& options, ns::AutoReleasedError* error)
    {
        Validate();
		NSString* nsSource = [NSString stringWithUTF8String:source];
		return m_table->NewLibraryWithSourceOptionsError(m_ptr, nsSource, options.GetPtr(), error ? (NSError**)error->GetInnerPtr() : nullptr);
    }
	
	Library Device::NewLibrary(ns::URL const& url, ns::AutoReleasedError* error)
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return m_table->NewLibraryWithURL(m_ptr, url.GetPtr(), error ? (NSError**)error->GetInnerPtr() : nullptr);
#else
		return Library();
#endif
	}

    void Device::NewLibrary(const char* source, const CompileOptions& options, LibraryHandler completionHandler)
    {
        Validate();
		NSString* nsSource = [NSString stringWithUTF8String:source];
		m_table->NewLibraryWithSourceOptionsCompletionHandler(m_ptr, nsSource, options.GetPtr(), ^(id <MTLLibrary> library, NSError * error) {
			completionHandler(
							  library,
							  error);
		});
    }

    RenderPipelineState Device::NewRenderPipelineState(const RenderPipelineDescriptor& descriptor, ns::AutoReleasedError* error)
    {
        Validate();
		return m_table->NewRenderPipelineStateWithDescriptorError(m_ptr, descriptor.GetPtr(), error ? (NSError**)error->GetInnerPtr() : nullptr);
    }

    RenderPipelineState Device::NewRenderPipelineState(const RenderPipelineDescriptor& descriptor, PipelineOption options, AutoReleasedRenderPipelineReflection* outReflection, ns::AutoReleasedError* error)
    {
        Validate();
		return m_table->NewRenderPipelineStateWithDescriptorOptionsReflectionError(m_ptr, descriptor.GetPtr(), MTLPipelineOption(options), outReflection ? outReflection->GetInnerPtr() : nil, error ? (NSError**)error->GetInnerPtr() : nullptr );
    }

    void Device::NewRenderPipelineState(const RenderPipelineDescriptor& descriptor, RenderPipelineStateHandler completionHandler)
    {
        Validate();
		m_table->NewRenderPipelineStateWithDescriptorCompletionHandler(m_ptr, descriptor.GetPtr(), ^(id <MTLRenderPipelineState> renderPipelineState, NSError * error) {
			completionHandler(
							  renderPipelineState,
							  error
							  );
		});
    }

    void Device::NewRenderPipelineState(const RenderPipelineDescriptor& descriptor, PipelineOption options, RenderPipelineStateReflectionHandler completionHandler)
    {
        Validate();
		m_table->NewRenderPipelineStateWithDescriptorOptionsCompletionHandler(m_ptr, descriptor.GetPtr(), MTLPipelineOption(options), ^(id <MTLRenderPipelineState> renderPipelineState, MTLRenderPipelineReflection * reflection, NSError * error) {
			completionHandler(
							  renderPipelineState,
							  reflection,
							  error
							  );
		});
    }

    ComputePipelineState Device::NewComputePipelineState(const Function& computeFunction, ns::AutoReleasedError* error)
    {
        Validate();
		return m_table->NewComputePipelineStateWithFunctionError(m_ptr, computeFunction.GetPtr(), error ? (NSError**)error->GetInnerPtr() : nullptr);
    }

    ComputePipelineState Device::NewComputePipelineState(const Function& computeFunction, PipelineOption options, AutoReleasedComputePipelineReflection& outReflection, ns::AutoReleasedError* error)
    {
        Validate();
		return m_table->NewComputePipelineStateWithFunctionOptionsReflectionError(m_ptr, computeFunction.GetPtr(), MTLPipelineOption(options), outReflection.GetInnerPtr(), error ? error->GetInnerPtr() : nil);
    }

    void Device::NewComputePipelineState(const Function& computeFunction, ComputePipelineStateHandler completionHandler)
    {
        Validate();
		m_table->NewComputePipelineStateWithFunctionCompletionHandler(m_ptr, computeFunction.GetPtr(), ^(id <MTLComputePipelineState> computePipelineState, NSError * error) {
			completionHandler(
							  computePipelineState,
							  error
							  );
		});
    }

    void Device::NewComputePipelineState(const Function& computeFunction, PipelineOption options, ComputePipelineStateReflectionHandler completionHandler)
    {
        Validate();
		m_table->NewComputePipelineStateWithFunctionOptionsCompletionHandler(m_ptr, computeFunction.GetPtr(), MTLPipelineOption(options), ^(id <MTLComputePipelineState> computePipelineState, MTLComputePipelineReflection * reflection, NSError * error) {
			completionHandler(
							  computePipelineState,
							  reflection,
							  error
							  );
		});
    }

    ComputePipelineState Device::NewComputePipelineState(const ComputePipelineDescriptor& descriptor, PipelineOption options, AutoReleasedComputePipelineReflection* outReflection, ns::AutoReleasedError* error)
    {
        Validate();
#if MTLPP_IS_AVAILABLE(10_11, 9_0)
		return m_table->NewComputePipelineStateWithDescriptorOptionsReflectionError(m_ptr, descriptor.GetPtr(), MTLPipelineOption(options), outReflection ? outReflection->GetInnerPtr() : nullptr, error ? error->GetInnerPtr() : nullptr);
#else
        return nullptr;
#endif
    }

    void Device::NewComputePipelineState(const ComputePipelineDescriptor& descriptor, PipelineOption options, ComputePipelineStateReflectionHandler completionHandler)
    {
        Validate();
#if MTLPP_IS_AVAILABLE(10_11, 9_0)
		m_table->NewComputePipelineStateWithDescriptorOptionsCompletionHandler(m_ptr,descriptor.GetPtr(), MTLPipelineOption(options), ^(id <MTLComputePipelineState> computePipelineState, MTLComputePipelineReflection * reflection, NSError * error)
																			   {
																				   completionHandler(
																									 computePipelineState,
																									 reflection,
																									 error);
																			   });
#endif
    }

    Fence Device::NewFence()
    {
        Validate();
#if MTLPP_IS_AVAILABLE(10_13, 10_0)
		return m_table->NewFence(m_ptr);
#else
		return nullptr;
#endif
    }

    bool Device::SupportsFeatureSet(FeatureSet featureSet) const
    {
        Validate();
		return m_table->supportsFeatureSet(m_ptr, MTLFeatureSet(featureSet));
    }

    bool Device::SupportsTextureSampleCount(NSUInteger sampleCount) const
    {
        Validate();
#if MTLPP_IS_AVAILABLE(10_11, 9_0)
		return m_table->supportsTextureSampleCount(m_ptr, sampleCount);
#else
        return true;
#endif
    }
	
	NSUInteger Device::GetMinimumLinearTextureAlignmentForPixelFormat(PixelFormat format) const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return m_table->minimumLinearTextureAlignmentForPixelFormat(m_ptr, (MTLPixelFormat)format);
#else
		return 0;
#endif
	}
	
	NSUInteger Device::GetMaxThreadgroupMemoryLength() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return m_table->maxThreadgroupMemoryLength(m_ptr);
#else
		return 0;
#endif
	}
	
	bool Device::AreProgrammableSamplePositionsSupported() const
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return m_table->areProgrammableSamplePositionsSupported(m_ptr);
#else
		return false;
#endif
	}
	
	void Device::GetDefaultSamplePositions(SamplePosition* positions, NSUInteger count)
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		m_table->getDefaultSamplePositionscount(m_ptr, positions, count);
#endif
	}
	
	ArgumentEncoder Device::NewArgumentEncoderWithArguments(ns::Array<ArgumentDescriptor> const& arguments)
	{
		Validate();
#if MTLPP_IS_AVAILABLE(10_13, 11_0)
		return m_table->NewArgumentEncoderWithArguments(m_ptr, (NSArray<MTLArgumentDescriptor*>*)arguments.GetPtr());
#else
		return ArgumentEncoder();
#endif
	}
	
	RenderPipelineState Device::NewRenderPipelineState(const TileRenderPipelineDescriptor& descriptor, PipelineOption options, AutoReleasedRenderPipelineReflection* outReflection, ns::AutoReleasedError* error)
	{
		Validate();
#if MTLPP_IS_AVAILABLE_IOS(11_0)
		return m_table->newRenderPipelineStateWithTileDescriptoroptionsreflectionerror(m_ptr, descriptor.GetPtr(), (MTLPipelineOption)options, outReflection ? outReflection->GetInnerPtr() : nullptr, error ? error->GetInnerPtr() : nullptr);
#else
		return RenderPipelineState();
#endif
	}
	
	void Device::NewRenderPipelineState(const TileRenderPipelineDescriptor& descriptor, PipelineOption options, RenderPipelineStateReflectionHandler completionHandler)
	{
		Validate();
#if MTLPP_IS_AVAILABLE_IOS(11_0)
		m_table->newRenderPipelineStateWithTileDescriptoroptionscompletionHandler(m_ptr, descriptor.GetPtr(), (MTLPipelineOption)options, ^(id <MTLRenderPipelineState> renderPipelineState, MTLRenderPipelineReflection * reflection, NSError * error) {
			completionHandler(
							  renderPipelineState,
							  reflection,
							  error
							  );
		});
#endif
	}
}

MTLPP_END
