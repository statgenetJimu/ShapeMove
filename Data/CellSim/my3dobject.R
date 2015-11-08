# 3DObjectは原点(0,0,0)を中心とした半径10の球に、次の３つの球が付着したもの
# (3,0,0)を中心とした半径5の球
# (0,5,0)を中心とした半径5の球
# (0,0,8)を中心とした半径5の球

# 各ピクセルの値は、xy正方形の４頂点のうちオブジェクト内に入っている点の割合とする

# このオブジェクトは移動する。移動はアフィン変換で表す
# アフィン変換は4x4行列、拡縮(Mag;vector with 3 values)、3D回転(Rot;3x3 matrix)、平行移動(Parallel;vector with 3 values)、剪断(Proj:Vector with 3 values)の合成で表される
#source("http://bioconductor.org/biocLite.R")
#biocLite("EBImage")
library(EBImage)
my.rot <- function(theta,phi){
	ret <- diag(rep(1,3))
	tmp1 <- ret
	tmp2 <- ret
	tmp1[1:2,1:2] <- matrix(c(cos(theta),sin(theta),-sin(theta),cos(theta)),2,2)
	tmp2[2:3,2:3] <- matrix(c(cos(phi),sin(phi),-sin(phi),cos(phi)),2,2)
	return(tmp2 %*% tmp1 %*% ret)
}


my.affin4x4 <- function(Rot, Mag, Parallel,Proj){
	ret <- diag(c(Mag,1))
	ret[1:3,1:3] <- Rot %*% ret[1:3,1:3]
	ret[1:3,4] <- Parallel
	ret[4,1:3] <- Proj
	return(ret)
} 

my.fx.t <- function(t,v1=1,v2=0,v3=1,v4=1){
	return(v1* t + v2* cos(v3*t+v4))
}

my.affin4x4 <- function(t,v.theta,v.phi,v.mag1,v.mag2,v.mag3,v.par1,v.par2,v.par3,v.proj1,v.proj2,v.proj3){
	theta <- my.fx.t(t,v.theta[1],v.theta[2],v.theta[3],v.theta[4])
	phi <-    my.fx.t(t,v.phi[1],v.phi[2],v.phi[3],v.phi[4])
	mag1 <- my.fx.t(t,v.mag1[1],v.mag1[2],v.mag1[3],v.mag1[4]) + 1
	mag2 <- my.fx.t(t,v.mag2[1],v.mag2[2],v.mag2[3],v.mag2[4]) + 1
	mag3 <- my.fx.t(t,v.mag3[1],v.mag3[2],v.mag3[3],v.mag3[4]) + 1
	par1 <- my.fx.t(t,v.par1[1],v.par1[2],v.par1[3],v.par1[4])
	par2 <- my.fx.t(t,v.par2[1],v.par2[2],v.par2[3],v.par2[4])
	par3 <- my.fx.t(t,v.par3[1],v.par3[2],v.par3[3],v.par3[4])
	proj1 <- my.fx.t(t,v.proj1[1],v.proj1[2],v.proj1[3],v.proj1[4])
	proj2 <- my.fx.t(t,v.proj2[1],v.proj2[2],v.proj2[3],v.proj2[4])
	proj3 <- my.fx.t(t,v.proj3[1],v.proj3[2],v.proj3[3],v.proj3[4])
	ret <- list()
	for(i in 1:length(t)){
		Mag <- c(mag1[i],mag2[i],mag3[i])
		Rot <-  my.rot(theta[i],phi[i])
		Parallel <- c(par1[i],par2[i],par3[i])
		Proj <- c(proj1[i],proj2[i],proj3[i])
		ret[[i]] <- diag(c(Mag,1))
		ret[[i]][1:3,1:3] <- Rot %*% ret[[i]][1:3,1:3]
		ret[[i]][1:3,4] <- Parallel
		ret[[i]][4,1:3] <- Proj
	}
	return(ret)
}

xs <- ys <- (-127):128
xys <- as.matrix(expand.grid(xs,ys))
zs <- seq(-10,10,by=2)
ts <- 1:20

v.theta <- c(0.1,0.0,0.1,0)
v.phi <- c(0,0.0,0.1,0)
v.mag1 <- c(0,0,0,0)
v.mag2 <- c(0,0,0,0)
v.mag3 <- c(0,0,0,0)
v.par1 <- c(0.1,0.1,1,0)
v.par2 <- c(0.1,0.1,3,0)
v.par3 <- c(0,0.1,0.1,0)
v.proj1 <- v.proj2 <- v.proj3 <- c(0,0,0,0)

Afs <- my.affin4x4(ts,v.theta,v.phi,v.mag1,v.mag2,v.mag3,v.par1,v.par2,v.par3,v.proj1,v.proj2,v.proj3)

zts <- expand.grid(1:length(zs),1:length(ts))

L1 <- 3
L2 <- 12
L3 <- 8
R0 <- R1 <- R2 <- R3 <- 8

for(i in 1:length(zts[,1])){
	z <- zs[zts[i,1]]
	t <- zts[i,2]
	xyz <- cbind(xys,z,1)
	xyz. <- t(Afs[[t]] %*% t(xyz))
	xyz. <- xyz.[,1:3]/xyz.[,4]
	b0 <- apply(xyz.^2,1,sum)
	b1 <- apply((cbind(xyz.[,1]-L1,xyz.[,2:3]))^2,1,sum)
	b2 <- apply((cbind(xyz.[,2]-L2,xyz.[,c(1,3)]))^2,1,sum)
	b3 <- apply((cbind(xyz.[,3]-L3,xyz.[,1:2]))^2,1,sum)
	tmp <- (b0 < R0^2 | b1 < R1^2 | b2 < R2^2 | b3 < R3^2)
	im <- Image(matrix(as.numeric(tmp),length(xs),length(ys)))
	filename <- paste("hoget",t,"_z",zts[i,1],".jpg",sep="")
	writeImage(im,filename)
} 