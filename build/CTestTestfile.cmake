# CMake generated Testfile for 
# Source directory: /home/stefano/Desktop/mesh-simplification
# Build directory: /home/stefano/Desktop/mesh-simplification/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(simp2dTest "simp2dTest")
add_test(evaluationCostFunction "evaluationCostFunction")
subdirs(RobustPredicates)
subdirs(src)
