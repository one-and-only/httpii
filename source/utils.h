#ifndef HTTPII_UTILS_H
#define HTTPII_UTILS_H

namespace HTTPii::Utils
{
    /**
     * @brief Clear the screen
     */
    extern void clear_screen();

    constexpr uint32_t hash(const char *data, size_t const size) noexcept
    {
        uint32_t hash = 5381;

        for (const char *c = data; c < data + size; c++)
            hash = ((hash << 5) + hash) + (unsigned char)*c;

        return hash;
    }
}

#endif