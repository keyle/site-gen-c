#define for_in(item, array, length) \
    for (int keep = 1,              \
             count = 0,             \
             size = length;         \
         keep && count != size;     \
         keep = !keep, count++)     \
        for (item = (array)[count]; keep; keep = !keep)
        
