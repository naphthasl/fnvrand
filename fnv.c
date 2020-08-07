uint32_t fnv1a(const void *data, uint32_t numBytes)
{
    uint32_t hash = 0x811C9DC5;
    const uint8_t *ptr = (const uint8_t *)data;
    
    while (numBytes--)
        hash = (*ptr++ ^ hash) * 0x01000193;

    return hash;
}

uint64_t fnv1a64(const void *data, uint64_t numBytes)
{
    uint64_t hash = 0xCBF29CE484222325;
    const uint8_t *ptr = (const uint8_t *)data;
    
    while (numBytes--)
        hash = (*ptr++ ^ hash) * 0x00000100000001B3;
        
    return hash;
}