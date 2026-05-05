find_package(Qt6 REQUIRED COMPONENTS Test Core Sql)

# ---------- Message store test ------------------------------------------
qt_add_executable(tst_store
    tst_store.cpp
    ../src/messagestore.cpp ../src/messagestore.h
    ../src/messagemodel.cpp ../src/messagemodel.h
)
target_include_directories(tst_store PRIVATE ../src)
target_link_libraries(tst_store PRIVATE
    Qt6::Test Qt6::Core Qt6::Sql
)
add_test(NAME tst_store COMMAND tst_store)
