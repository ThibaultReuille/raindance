#pragma once

#include <raindance/Core/Headers.hh>

class OpenCL
{
public:
    struct Platform
    {
        cl_platform_id ID;
        std::string Profile;
        std::string Version;
        std::string Name;
        std::string Vendor;
        std::string Extensions;
    };

    struct Device
    {
        cl_device_id ID;
        std::string Name;
        cl_platform_id Platform;
        std::string Profile;
        cl_device_type Type;
        std::string Vendor;
        std::string Version;
        std::string DriverVersion;
        std::string Extensions;
    };

    struct Context
    {
        cl_context Object;
        cl_platform_id Platform;
        cl_device_id Device;
    };

    struct CommandQueue
    {
        cl_command_queue Object;
    };

    struct Program
    {
        cl_program Object;
    };

    struct Memory
    {
        cl_mem Object;
        cl_context Context;
        size_t Size;
    };

    struct Kernel
    {
        cl_kernel Object;
        cl_program Program;
        std::string Name;

        void setArgument(cl_uint index, const Memory& buffer) const
        {
            cl_int error = clSetKernelArg(Object, index, sizeof(cl_mem), &buffer.Object);
            if (error != CL_SUCCESS)
            {
                showError(error);
                throw;
            }
        }

        void setArgument(cl_uint index, const void* data, size_t size) const
        {
            cl_int error = clSetKernelArg(Object, index, size, data);
            if (error != CL_SUCCESS)
            {
                showError(error);
                throw;
            }
        }

        void showError(cl_int error) const
        {
            LOG("[OpenCL] Error: Failed to set kernel argument -> ");
            switch(error)
            {
            case CL_INVALID_KERNEL:
                LOG("CL_INVALID_KERNEL\n");
                break;
            case CL_INVALID_ARG_INDEX:
                LOG("CL_INVALID_ARG_INDEX\n");
                break;
            case CL_INVALID_ARG_VALUE:
                LOG("CL_INVALID_ARG_VALUE\n");
                break;
            case CL_INVALID_MEM_OBJECT:
                LOG("CL_INVALID_MEM_OBJECT\n");
                break;
            case CL_INVALID_SAMPLER:
                LOG("CL_INVALID_SAMPLER\n");
                break;
            case CL_INVALID_ARG_SIZE:
                LOG("CL_INVALID_ARG_SIZE\n");
                break;
            default:
                LOG("Unknown\n");
            }
        }
    };

    OpenCL()
    {
    }

    virtual ~OpenCL()
    {
    }

    void detect()
    {
        cl_int error;
        cl_uint platformCount;
        cl_platform_id* platformIDs = NULL;

        error = clGetPlatformIDs(0, NULL, &platformCount);
        if (error != CL_SUCCESS)
        {
            LOG("[OpenCL] Error: %i code in clGetPlatformIDs call!\n", error);
            return;
        }
        if (platformCount == 0)
        {
            LOG("[OpenCL] Error: No OpenCL platform found!\n");
            return;
        }
        platformIDs = new cl_platform_id[platformCount];
        error = clGetPlatformIDs(platformCount, platformIDs, NULL);

        for (cl_uint i = 0; i < platformCount; i++)
        {
            Platform* platform = new Platform();
            platform->ID = platformIDs[i];
            platform->Profile = getPlatformInfoString(platformIDs[i], CL_PLATFORM_PROFILE);
            platform->Version = getPlatformInfoString(platformIDs[i], CL_PLATFORM_VERSION);
            platform->Name = getPlatformInfoString(platformIDs[i], CL_PLATFORM_NAME);
            platform->Vendor = getPlatformInfoString(platformIDs[i], CL_PLATFORM_VENDOR);
            platform->Extensions = getPlatformInfoString(platformIDs[i], CL_PLATFORM_EXTENSIONS);

            m_Platforms.push_back(platform);

            cl_uint deviceCount;
            cl_device_id* deviceIDs = NULL;
            error = clGetDeviceIDs(platformIDs[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
            if (error != CL_SUCCESS)
            {
                LOG("[OpenCL] Error: Failed to get device IDs!\n");
                return;
            }
            if (deviceCount == 0)
            {
                LOG("[OpenCL] Error: No OpenCL device found for platform %p !\n", platformIDs[i]);
                return;
            }
            deviceIDs = new cl_device_id[deviceCount];
            error = clGetDeviceIDs(platformIDs[i], CL_DEVICE_TYPE_ALL, deviceCount, deviceIDs, NULL);

            for (cl_uint j = 0; j < deviceCount; j++)
            {
                Device* device = new Device();
                
                device->ID = deviceIDs[j];
                
                device->Name = getDeviceInfoString(deviceIDs[j], CL_DEVICE_NAME);
                
                clGetDeviceInfo(deviceIDs[j], CL_DEVICE_PLATFORM, sizeof(cl_platform_id), &device->Platform, NULL);
                
                device->Profile = getDeviceInfoString(deviceIDs[j], CL_DEVICE_PROFILE);
                
                clGetDeviceInfo(deviceIDs[j], CL_DEVICE_TYPE, sizeof(cl_device_type), &device->Type, NULL);
                
                device->Vendor = getDeviceInfoString(deviceIDs[j], CL_DEVICE_VENDOR);
                
                device->Version = getDeviceInfoString(deviceIDs[j], CL_DEVICE_VERSION);

                device->DriverVersion = getDeviceInfoString(deviceIDs[j], CL_DRIVER_VERSION);

                device->Extensions = getDeviceInfoString(deviceIDs[j], CL_DEVICE_EXTENSIONS);

                m_Devices.push_back(device);
            }

            delete[] deviceIDs;
        }

        delete[] platformIDs;

        LOG("[OpenCL] %i platform(s) and %i device(s) detected.\n", static_cast<int>(m_Platforms.size()), static_cast<int>(m_Devices.size()));
    }

    Context* createContext(const Device& device)
    {
        LOG("[OpenCL] Creating context using %s ...\n", device.Name.c_str());

        cl_int error;

        #if defined (__APPLE__) || defined(MACOSX)

            CGLContextObj kCGLContext = CGLGetCurrentContext(); 
            CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);

            cl_context_properties properties[] =
            {
                CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
                (cl_context_properties) kCGLShareGroup,
                0
            };

        #else
        # if defined (__linux__)

            cl_context_properties properties[] =
            {
                CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
                CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
                CL_CONTEXT_PLATFORM, (cl_context_properties) device.Platform, 
                0
            }; 

        # else

            cl_context_properties properties[] =
            {
                CL_CONTEXT_PLATFORM,
                (cl_context_properties) device.Platform,
                0
            }; 

        # endif
        #endif

        cl_context object = clCreateContext(properties, 1, &device.ID, NULL, NULL, &error);
        if (error != CL_SUCCESS)
        {
            LOG("[OpenCL] Error: Failed to create a compute context!\n");
            throw;
        }

        Context* context = new Context();
        context->Object = object;
        context->Platform = device.Platform;
        context->Device = device.ID;

        m_Contexts.push_back(context);

        return context;
    }

    CommandQueue* createCommandQueue(const Context& context)
    {
        LOG("[OpenCL] Creating command queue ...\n");

        cl_int error;

        cl_command_queue object = clCreateCommandQueue(context.Object, context.Device, 0, &error);
        if (error != CL_SUCCESS)
        {
            LOG("[OpenCL] Error: Failed to create a command queue!\n");
            throw;
        }

        CommandQueue* queue = new CommandQueue();
        queue->Object = object;

        m_Queues.push_back(queue);

        return queue;
    }

    // Creates and builds an OpenCL program from source
    Program* loadProgram(const Context& context, const char* name, const std::string& source)
    {
        cl_int error;
        const char* src = source.c_str();

        LOG("[OpenCL] Building program '%s' from source ...\n", name);

        cl_program object = clCreateProgramWithSource(context.Object, 1, &src, NULL, &error);
        if (error != CL_SUCCESS)
        {
            LOG("[OpenCL] Error: Failed to create compute program!\n");
            throw;
        }

        error = clBuildProgram(object, 0, NULL, NULL, NULL, NULL);
        if (error != CL_SUCCESS)
        {
            size_t len;
            char buffer[2048];
            LOG("[OpenCL] Error: Failed to build program executable!\n");
            clGetProgramBuildInfo(object, context.Device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
            LOG("%s\n", buffer);
            throw;
        }

        Program* program = new Program();
        program->Object = object;

        m_Programs.push_back(program);

        return program;
    }

    Kernel* createKernel(const Program& program, const char* name)
    {
        cl_int error;

        LOG("[OpenCL] Creating kernel '%s' ...\n", name);
        cl_kernel object = clCreateKernel(program.Object, name, &error);
        if (!object || error != CL_SUCCESS)
        {
            LOG("[OpenCL] Error: Failed to create compute kernel '%s'!\n", name);
            throw;
        }

        Kernel* kernel = new Kernel();
        kernel->Object = object;
        kernel->Program = program.Object;
        kernel->Name = std::string(name);

        m_Kernels.push_back(kernel);

        return kernel;
    }

    Memory* createBuffer(const Context& context, cl_mem_flags flags, size_t size)
    {
        cl_int error;

        LOG("[OpenCL] Creating buffer <%zu>' ...\n", size);
        cl_mem object = clCreateBuffer(context.Object, flags, size, NULL, &error);
        if (error != CL_SUCCESS)
        {
            LOG("[OpenCL] Error: Failed to create buffer!\n");
            throw;
        }

        Memory* buffer = new Memory();
        buffer->Object = object;
        buffer->Context = context.Object;
        buffer->Size = size;

        m_Buffers.push_back(buffer);

        return buffer;
    }

    Memory* createFromGLBuffer(const Context& context, cl_mem_flags flags, GLuint bufobj)
    {
         cl_int error;

        LOG("[OpenCL] Creating buffer from GL VBO<%u>' ...\n", bufobj);
        cl_mem object = clCreateFromGLBuffer(context.Object, flags, bufobj, &error);
        if (error != CL_SUCCESS)
        {
            LOG("[OpenCL] Error: Failed to create buffer!\n");
            throw;
        }

        Memory* buffer = new Memory();
        buffer->Object = object;
        buffer->Context = context.Object;
        buffer->Size = 0; // NOTE : 0 = From GL Buffer

        m_Buffers.push_back(buffer);

        return buffer;   
    }

    void destroyBuffer(Memory** memory)
    {
        LOG("[OpenCL] Destroying buffer %p\n", *memory);
        clReleaseMemObject((*memory)->Object);
        delete *memory;
        *memory = NULL;
    }

    void enqueueWriteBuffer(const CommandQueue& queue,
                            const Memory& buffer,
                            cl_bool blocking_write,
                            size_t offset,
                            size_t cb,
                            const void *ptr,
                            cl_uint num_events_in_wait_list,
                            const cl_event *event_wait_list,
                            cl_event *event)
    {
        cl_int error = clEnqueueWriteBuffer(queue.Object, buffer.Object, blocking_write, offset, cb, ptr, num_events_in_wait_list, event_wait_list, event);
        if (error != CL_SUCCESS)
        {
            LOG("Error: Failed to write in buffer -> ");
            switch(error)
            {
            case CL_INVALID_COMMAND_QUEUE:
                LOG("CL_INVALID_COMMAND_QUEUE\n");
                break;
            case CL_INVALID_CONTEXT:
                LOG("CL_INVALID_CONTEXT\n");
                break;
            case CL_INVALID_MEM_OBJECT:
                LOG("CL_INVALID_MEM_OBJECT\n");
                break;
            case CL_INVALID_VALUE:
                LOG("CL_INVALID_VALUE\n");
                break;
            case CL_INVALID_EVENT_WAIT_LIST:
                LOG("CL_INVALID_EVENT_WAIT_LIST\n");
                break;
            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                LOG("CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
                break;
            case CL_OUT_OF_HOST_MEMORY:
                LOG("CL_OUT_OF_HOST_MEMORY\n");
                break;
            default:
                LOG("Unknown\n");
                break;
            }
            throw;
        }
    }

    void enqueueNDRangeKernel(const CommandQueue& queue,
                              const Kernel& kernel,
                              cl_uint work_dim,
                              const size_t *global_work_offset,
                              const size_t *global_work_size,
                              const size_t *local_work_size,
                              cl_uint num_events_in_wait_list,
                              const cl_event *event_wait_list,
                              cl_event *event)
    {
        cl_int error = clEnqueueNDRangeKernel(queue.Object, kernel.Object, work_dim, global_work_offset, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);
        if (error != CL_SUCCESS)
        {
            LOG("[OpenCL] Error: Failed to execute kernel : ");
            switch(error)
            {
            case CL_INVALID_PROGRAM_EXECUTABLE:
                LOG("CL_INVALID_PROGRAM_EXECUTABLE\n");
                break;
            case CL_INVALID_COMMAND_QUEUE:
                LOG("CL_INVALID_COMMAND_QUEUE\n");
                break;
            case CL_INVALID_KERNEL:
                LOG("CL_INVALID_KERNEL\n");
                break;
            case CL_INVALID_CONTEXT:
                LOG("CL_INVALID_CONTEXT\n");
                break;
            case CL_INVALID_KERNEL_ARGS:
                LOG("CL_INVALID_KERNEL_ARGS\n");
                break;
            case CL_INVALID_WORK_DIMENSION:
                LOG("CL_INVALID_WORK_DIMENSION\n");
                break;
            case CL_INVALID_WORK_GROUP_SIZE:
                LOG("CL_INVALID_WORK_GROUP_SIZE\n");
                break;
            case CL_INVALID_WORK_ITEM_SIZE:
                LOG("CL_INVALID_WORK_ITEM_SIZE\n");
                break;
            case CL_INVALID_GLOBAL_OFFSET:
                LOG("CL_INVALID_GLOBAL_OFFSET\n");
                break;
            case CL_OUT_OF_RESOURCES:
                LOG("CL_OUT_OF_RESOURCES\n");
                break;
            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                LOG("CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
                break;
            case CL_INVALID_EVENT_WAIT_LIST:
                LOG("CL_INVALID_EVENT_WAIT_LIST\n");
                break;
            case CL_OUT_OF_HOST_MEMORY:
                LOG("CL_OUT_OF_HOST_MEMORY\n");
                break;
            default:
                LOG("Unknown\n");
                break;
            }
            throw;
        }
    }

    void enqueueReadBuffer(const CommandQueue& queue,
                           const Memory& buffer,
                           cl_bool blocking_read,
                           size_t offset,
                           size_t cb,
                           void *ptr,
                           cl_uint num_events_in_wait_list,
                           const cl_event *event_wait_list,
                           cl_event *event)
    {
        cl_int error = clEnqueueReadBuffer(queue.Object, buffer.Object, blocking_read, offset, cb, ptr, num_events_in_wait_list, event_wait_list, event);
        if (error != CL_SUCCESS)
        {
            LOG("[OpenCL] Error: Failed to read buffer -> %d", error);

            switch(error)
            {
            case CL_INVALID_COMMAND_QUEUE:
                LOG("CL_INVALID_COMMAND_QUEUE\n");
                break;
            case CL_INVALID_CONTEXT:
                LOG("CL_INVALID_CONTEXT\n");
                break;
            case CL_INVALID_MEM_OBJECT:
                LOG("CL_INVALID_MEM_OBJECT\n");
                break;
            case CL_INVALID_VALUE:
                LOG("CL_INVALID_VALUE\n");
                break;
            case CL_INVALID_EVENT_WAIT_LIST:
                LOG("CL_INVALID_EVENT_WAIT_LIST\n");
                break;
            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                LOG("CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
                break;
            case CL_OUT_OF_HOST_MEMORY:
                LOG("CL_OUT_OF_HOST_MEMORY\n");
                break;
            default:
                break;
            }
            throw;
        }
    }



    void enqueueAcquireGLObjects(const CommandQueue& queue,
                                 cl_uint num_objects,
                                 const cl_mem *mem_objects,
                                 cl_uint num_events_in_wait_list,
                                 const cl_event *event_wait_list,
                                 cl_event *event)
    {
        cl_int error = clEnqueueAcquireGLObjects(queue.Object, num_objects, mem_objects, num_events_in_wait_list, event_wait_list, event);
        if (error != CL_SUCCESS)
        {
            switch(error)
            {
            case CL_INVALID_VALUE:
                LOG("CL_INVALID_VALUE\n");
                break;
            case CL_INVALID_MEM_OBJECT:
                LOG("CL_INVALID_MEM_OBJECT\n");
                break;
            case CL_INVALID_COMMAND_QUEUE:
                LOG("CL_INVALID_COMMAND_QUEUE\n");
                break;
            case CL_INVALID_CONTEXT:
                LOG("CL_INVALID_CONTEXT\n");
                break;
            case CL_INVALID_GL_OBJECT:
                LOG("CL_INVALID_GL_OBJECT\n");
                break;
            case CL_INVALID_EVENT_WAIT_LIST:
                LOG("CL_INVALID_EVENT_WAIT_LIST\n");
                break;
            case CL_OUT_OF_RESOURCES:
                LOG("CL_OUT_OF_RESOURCES\n");
                break;
            case CL_OUT_OF_HOST_MEMORY: 
                LOG("CL_OUT_OF_HOST_MEMORY\n");
                break;
            default:
                break;
            }
            throw;
        }
    }   

    void enqueueReleaseGLObjects(const CommandQueue& queue,
                                   cl_uint num_objects,
                                   const cl_mem *mem_objects,
                                   cl_uint num_events_in_wait_list,
                                   const cl_event *event_wait_list,
                                   cl_event *event)
    {
        cl_int error = clEnqueueReleaseGLObjects(queue.Object, num_objects, mem_objects, num_events_in_wait_list, event_wait_list, event);
        if (error != CL_SUCCESS)
        {
            switch(error)
            {
            case CL_INVALID_VALUE:
                LOG("CL_INVALID_VALUE\n");
                break;
            case CL_INVALID_MEM_OBJECT:
                LOG("CL_INVALID_MEM_OBJECT\n");
                break;
            case CL_INVALID_COMMAND_QUEUE:
                LOG("CL_INVALID_COMMAND_QUEUE\n");
                break;
            case CL_INVALID_CONTEXT:
                LOG("CL_INVALID_CONTEXT\n");
                break;
            case CL_INVALID_GL_OBJECT:
                LOG("CL_INVALID_GL_OBJECT\n");
                break;
            case CL_INVALID_EVENT_WAIT_LIST:
                LOG("CL_INVALID_EVENT_WAIT_LIST\n");
                break;
            case CL_OUT_OF_RESOURCES:
                LOG("CL_OUT_OF_RESOURCES\n");
                break;
            case CL_OUT_OF_HOST_MEMORY: 
                LOG("CL_OUT_OF_HOST_MEMORY\n");
                break;
            default:
                break;
            }
            throw;
        }
    }   

    void dump(const Device& device)
    {
        LOG("%s (id: %p)\n", device.Name.c_str(), device.ID);

        LOG("\tType :           ");
        switch(device.Type)
        {
            case CL_DEVICE_TYPE_DEFAULT:
                LOG("Default\n");
                break;
            case CL_DEVICE_TYPE_CPU:
                LOG("CPU\n");
                break;
            case CL_DEVICE_TYPE_GPU:
                LOG("GPU\n");
                break;
            case CL_DEVICE_TYPE_ACCELERATOR:
                LOG("Accelerator\n");
                break;
            case CL_DEVICE_TYPE_ALL:
                LOG("All\n");
                break;
            default:
                LOG("Unknown\n");
                break;
        };

        LOG("\tVendor :         %s\n", device.Vendor.c_str());
        LOG("\tVersion :        %s\n", device.Version.c_str());
        LOG("\tDriverVersion :  %s\n", device.DriverVersion.c_str());
        LOG("\tProfile :        %s\n", device.Profile.c_str());
        LOG("\tPlatform :       %p\n", device.Platform);
        

        LOG("\tExtensions : %s\n", device.Extensions.c_str()); 
    }

    void dump(const Platform& platform)
    {
        LOG("%s (id: %p)\n", platform.Name.c_str(), platform.ID);
        LOG("\tVendor :      %s\n", platform.Vendor.c_str());
        LOG("\tVersion :     %s\n", platform.Version.c_str());
        LOG("\tProfile :     %s\n", platform.Profile.c_str());
    }

    void dump()
    {
        unsigned int count;

        count = 0;
        for (auto platform : m_Platforms)
        {
            LOG("\n[OpenCL] Platform #%u\n", count);
            dump(*platform);
            count++;
        }

        count = 0;
        for (auto device : m_Devices)
        {
            LOG("\n[OPENCL] Device #%u\n", count);
            dump(*device);
            count++;
        }
    }

    inline const std::vector<Platform*>& platforms() { return m_Platforms; }
    inline const Platform* platform(unsigned int id) { return m_Platforms[id]; }

    inline const std::vector<Device*>& devices() { return m_Devices; }
    inline const Device* device(unsigned int id) { return m_Devices[id]; }

    inline const std::vector<Context*>& contexts() { return m_Contexts; }
    inline const Context* context(unsigned int id) { return m_Contexts[id]; }

    inline const std::vector<CommandQueue*>& queues() { return m_Queues; }
    inline const CommandQueue* queue(unsigned int id) { return m_Queues[id]; }

    inline const std::vector<Program*>& programs() { return m_Programs; }
    inline const Program* program(unsigned int id) { return m_Programs[id]; }

    inline const std::vector<Kernel*>& kernels() { return m_Kernels; }
    inline const Kernel* kernel(unsigned int id) { return m_Kernels[id]; }

    inline const std::vector<Memory*>& buffers() { return m_Buffers; }
    inline const Memory* buffer(unsigned int id) { return m_Buffers[id]; }

private:
    std::string getPlatformInfoString(cl_platform_id id, cl_platform_info info)
    {
        std::string str;
        char *data = NULL;
        size_t size;

        clGetPlatformInfo(id, info, 0, data, &size);
        data = new char[size];
        clGetPlatformInfo(id, info, size, data, NULL);
        str = std::string(data);
        delete[] data;

        return str;
    }

    std::string getDeviceInfoString(cl_device_id id, cl_device_info info)
    {
        std::string str;
        char *data = NULL;
        size_t size;

        clGetDeviceInfo(id, info, 0, data, &size);
        data = new char[size];
        clGetDeviceInfo(id, info, size, data, NULL);
        str = std::string(data);
        delete[] data;

        return str;
    }

    std::vector<Platform*> m_Platforms;
    std::vector<Device*> m_Devices;
    std::vector<Context*> m_Contexts;
    std::vector<CommandQueue*> m_Queues;
    std::vector<Program*> m_Programs;
    std::vector<Kernel*> m_Kernels;
    std::vector<Memory*> m_Buffers;
};
