#############READ ME##############

please read this file before running the program.

Few instructions has to be considered before implementation

1)It is advicable to use the largest contour as the platform for placing objects.
2)Enter the actual dimensions of the largest contour used in the image and make sure orientation is taken into consideration
3)In order to calculate the fillet radius, certain changes has to be made to the set-up which is given in a seperate section.

Fillet Diameter scenario

The setup needs to be properly setup for getting accurate measurement of the fillet radii. The code has the algorithm to detect the radii accurately
but it will ultimately depend on the set up. The orientation of the platform and the object should be either perfectly horizontal or vertical. The camera
should be mounted in such a way that skewness is eliminated completely.