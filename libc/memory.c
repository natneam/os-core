void memory_copy(char* source, char* dest, int no_bytes)
{
    for (int i = 0; i < no_bytes; i++)
    {
        *(dest + i) = *(source + i);
    }
}

void* memset(void* buf, int c, unsigned int len) {
    unsigned char* p = buf;
    while (len-- > 0) {
        *p++ = (unsigned char)c;
    }
    return buf;
}