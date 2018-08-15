#pragma once
#include "Headers.inl"

// store in official namespace
namespace vrt {

    // any other vertex bindings can be used by attributes
    struct VtVertexRegionBinding {
        uint32_t byteOffset = 0;
        uint32_t byteSize = 0;
    };

    // buffer view
    struct VtVertexBufferView {
        uint32_t regionID = 0;
        uint32_t byteOffset = 0;
        uint32_t byteStride = 0;
        uint32_t byteLength = 0;
    };

    // attribute binding
    struct VtVertexAttributeBinding {
        uint32_t attributeBinding = 0;
        uint32_t accessorID = 0;
    };

    // system vectors of ray tracers
    struct VtVec4 { float x, y, z, w; };
    struct VtVec3 { float x, y, z; };
    struct VtVec2 { float x, y; };
    struct VtUVec2 { uint32_t x, y; };
    struct VtMat4 { VtVec4 m0, m1, m2, m3; };

    // in future planned custom ray structures support
    // in current moment we will using 32-byte standard structuring
    struct VtRay {
        VtVec3 origin; // position state (in 3D)
        int32_t hitID; // id of intersection hit (-1 is missing)
        VtVec2 cdirect; // polar direction
        uint32_t _indice; // reserved for indice in another ray system
        uint16_t hf_r, hf_g, hf_b, bitfield;
    };

    struct VtVirtualCombinedImage {
        union {
            uint64_t _combined = 0ull;
            struct { uint64_t textureID : 32, samplerID : 32; } _combination;
        };

        // per-component constructor
        VtVirtualCombinedImage(uint32_t textureID = 0u, uint32_t samplerID = 0u) : _combination({ textureID + 1u, samplerID + 1u }) {};
        VtVirtualCombinedImage(uint64_t combined) : _combined(combined) {};
        //VtVirtualCombinedImage() {};

        // component setters
        VtVirtualCombinedImage& setTextureID(uint32_t textureID = 0) { _combination.textureID = textureID + 1u; return *this; }
        VtVirtualCombinedImage& setSamplerID(uint32_t samplerID = 0) { _combination.samplerID = samplerID + 1u; return *this; }

        // component getters
        uint32_t getTextureID() const { return uint32_t(_combination.textureID) - 1u; }
        uint32_t getSamplerID() const { return uint32_t(_combination.samplerID) - 1u; }

        // casting operator
        operator uint64_t() const { return _combined; };
        operator uint64_t&() { return _combined; };
    };

    typedef enum VtType : uint32_t {
        VT_TYPE_FLOAT = 0,
        VT_TYPE_UINT32 = 1,
        VT_TYPE_UINT16 = 2,
        VT_TYPE_HALF = 3
    } VtType;

    // constexpr format compositor
    struct VtFormatDecomp {
        union { uint32_t _format = 0; struct { uint32_t _components : 2, _type : 4, _normalized : 1; } _formatDecomp; };

        constexpr VtFormatDecomp() {};
        constexpr VtFormatDecomp(uint8_t components, uint8_t type, uint8_t normalized = 0) : _formatDecomp({ components - 1u , type, normalized }) {};
        constexpr VtFormatDecomp(uint32_t format) : _format(format) {};
        constexpr operator uint32_t() const { return _format; };
        //operator uint32_t() const { return _format; };

        constexpr VtFormatDecomp& setComponents(uint8_t components) { _formatDecomp._components = components - 1u; return *this; };
        constexpr VtFormatDecomp& setType(uint8_t type) { _formatDecomp._type = type; return *this; };
        constexpr VtFormatDecomp& setNormalized(bool normalized) { _formatDecomp._normalized = normalized; return *this; };

        constexpr uint8_t getComponents() const { return _formatDecomp._components + 1u; };
        constexpr uint8_t getType() const { return _formatDecomp._type; };
        constexpr bool getNormalized() const { return _formatDecomp._normalized; };
    };

    // any other vertex accessors can be used by attributes
    struct VtVertexAccessor {
        uint32_t bufferViewID = 0;
        uint32_t byteOffset = 0;
        VtFormatDecomp format;
        uint32_t reserved = 0;
    };

    struct VtPrimitiveBitfield { uint32_t hitGroup : 2, frontFace : 1, backFace : 1; };

    struct VtUniformBlock {
        uint32_t primitiveCount = 0;
        uint32_t verticeAccessor = 0;
        uint32_t indiceAccessor = 0xFFFFFFFFu;
        uint32_t materialAccessor = 0xFFFFFFFFu;

        uint32_t primitiveOffset = 0;
        uint32_t attributeOffset = 0;
        uint32_t attributeCount = 8;
        union {
            uint32_t bitfield = 0u;
            VtPrimitiveBitfield bitfieldDetail;
        };

        uint32_t materialID = 0;
        uint32_t readOffset = 0;
        uint32_t reserved0 = 0, reserved1 = 0;
    };

    struct VtStageUniform { 
        int currentGroup = 0, maxRayCount = 0, iteration = 0, closestHitOffset = 0;
        int width = 1, height = 1, lastIteration = 0, r0 = 0;
    };

    struct VtBvhBlock {
        VtMat4 transform;
        VtMat4 transformInv;
        VtMat4 projection;
        VtMat4 projectionInv;
        int leafCount = 0, primitiveCount = 0, entryID = 0, primitiveOffset = 0;
    };
};
