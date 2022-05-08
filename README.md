# Dimension-Measurement-using-OpenCV-and-C++
The following code written in C++ using OpenCV library is used to measure the dimensions of basic objects with maximum accuracy. The code has been written to work in different
lighting conditions and any generalised set-up.

### Please read this file before running the program.

Few instructions has to be considered before implementation

#### It is advicable to use the largest contour as the platform for placing objects.
#### Enter the actual dimensions of the largest contour used in the image and make sure orientation is taken into consideration
#### In order to calculate the fillet radius, certain changes has to be made to the set-up which is given in a seperate section.

### Fillet Diameter scenario

The setup needs to be properly done for getting accurate measurement of the fillet radii. The code has the algorithm to detect the radii accurately
but it will ultimately depend on the set up. The orientation of the platform and the object should be either perfectly horizontal or vertical. The camera
should be mounted in such a way that skewness is eliminated completely.
