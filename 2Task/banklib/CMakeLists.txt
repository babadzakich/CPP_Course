add_library(banklib STATIC
    src/bankController.cpp
    src/staticStorage.cpp
    src/classes/Account.cpp
    src/classes/Bank_Account.cpp
        src/classes/Credit.cpp
    src/classes/Deposit.cpp
    src/classes/Exchange_Rate.cpp
    src/classes/Workplace.cpp
    src/enums/Account_Type.cpp
    src/enums/Client_Type.cpp
    src/enums/Credit_Type.cpp
    src/enums/Currency_Type.cpp
    src/enums/Deposit_Type.cpp
    src/enums/Workspace_Type.cpp
)

target_include_directories(banklib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS banklib
    EXPORT banklibTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT banklibTargets
    FILE banklibConfig.cmake
    NAMESPACE banklib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/banklib
)

# add_subdirectory(tests)
