add_library(
        vectorDB_index
        OBJECT
        faiss_index.cpp
        hnswlib_index.cpp
        index_factory.cpp
        filter_index.cpp
        )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:vectorDB_index>
        PARENT_SCOPE)