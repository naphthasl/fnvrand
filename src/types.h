#ifndef TYPES_HEADER_FILE
#define TYPES_HEADER_FILE
    enum R_TYPES {
        TYPE_CHAR,
        TYPE_UCHAR,
        TYPE_SHORT,
        TYPE_USHORT,
        TYPE_INT,
        TYPE_UINT,
        TYPE_LONG,
        TYPE_ULONG,
        TYPE_LONGLONG,
        TYPE_ULONGLONG,
        TYPE_R_BOOL,
        TYPE_R_TABLE,
        TYPE_R_LIST,
        TYPE_R_BLOB,
        TYPE_INT8,
        TYPE_INT16,
        TYPE_INT32,
        TYPE_INT64,
        TYPE_UINT8,
        TYPE_UINT16,
        TYPE_UINT32,
        TYPE_UINT64,
        TYPE_INT128,
        TYPE_UINT128,
        TYPE_R_FRACT,
        TYPE_R_POINT2D,
        TYPE_R_POINT3D,
        TYPE_R_MESH3D,
        TYPE_R_REGISTRY
    };

    enum R_TYPE_MODIFIER {
        TYPE_MOD_POINTER = 65535,
        TYPE_MOD_ARRAY = 32767
    };
#endif