unsigned int fnv1a(const void *data, unsigned int numBytes)
{
    unsigned int hash = 0x811C9DC5;
    const unsigned char *ptr = (const unsigned char *)data;
    
    while (numBytes--)
        hash = (*ptr++ ^ hash) * 0x01000193;

    return hash;
}

unsigned long long fnv1a64(
    const void *data,
    unsigned long long numBytes)
{
    unsigned long long hash = 0xCBF29CE484222325;
    const unsigned char *ptr = (const unsigned char *)data;
    
    while (numBytes--)
        hash = (*ptr++ ^ hash) * 0x00000100000001B3;
        
    return hash;
}