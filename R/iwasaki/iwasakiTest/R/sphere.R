#' sphere plus cone

#' @export
#' @examples
#' library(rgl)
#' sphere()

sphere <- function(){

	n <- 10000
	tmax <- 1
	smax <- 1

	t <- runif(n,-tmax,tmax)
	s <- runif(n,-smax,smax)

	T <- cumsum(t)
	S <- cumsum(s)

	x <- 0.1*(sin(T))*cos(S)
	y <- 0.1*(sin(T))*sin(S)
	z <- 0.1*cos(T)

	X <- 0.01*(sin(T))*cos(S)
	Y <- 0.01*(sin(T))*sin(S)
	Z <- 0.1-0.1*abs(sin(T))+0.1

	library(rgl)
	plot3d(rbind(cbind(x,y,z),cbind(X,Y,Z)),aspect=FALSE)
}

