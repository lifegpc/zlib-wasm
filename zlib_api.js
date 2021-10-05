
Module['version'] = Module['cwrap']('version', 'string', []);

function ci(arr, fun) {
    let free = Module["_free"];
    let malloc = Module["_malloc"];
    let HEAP8 = Module["HEAP8"];
    let HEAPU32 = Module["HEAPU32"];
    let ptr = malloc(arr.length);
    if (ptr == 0) {
        return null;
    }
    HEAP8.set(arr, ptr);
    let dptr = malloc(4);
    if (dptr == 0) {
        free(dptr);
        return null;
    }
    let dlenp = malloc(4);
    if (dlenp == 0) {
        free(ptr);
        free(dptr);
        return null;
    }
    if (fun(dptr, dlenp, ptr, arr.length) != 0) {
        free(ptr);
        free(dptr);
        free(dlenp);
        return null;
    }
    let le = HEAPU32[dlenp / 4];
    free(dlenp);
    let deptr = HEAPU32[dptr / 4];
    let de = new Int8Array(le);
    de.set(HEAP8['subarray'](deptr, deptr + le));
    free(ptr);
    free(dptr);
    free(deptr);
    return de;
}

Module['compress'] = function (arr) {
    return ci(arr, Module["_zlib_compress"]);
}

Module['uncompress'] = function (arr) {
    return ci(arr, Module["_zlib_uncompress"]);
}
