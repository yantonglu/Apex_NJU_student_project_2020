# If we don't need RTTI or EH, there's no reason to export anything
# from the hello plugin.
if( NOT LLVM_REQUIRES_RTTI )
  if( NOT LLVM_REQUIRES_EH )
    set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Hello.exports)
  endif()
endif()

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS Core Support)
endif()

add_llvm_library(LLVMHello MODULE BUILDTREE_ONLY
  mystuff1.cpp
  mystuff2.cpp
  ExpandConstantExpr.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
