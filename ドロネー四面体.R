library(geometry)
library(gtools)
## Not run: 
# example delaunayn
#d = c(-1,1)
n.pt <- 30
pc = matrix(rnorm(3*n.pt),ncol=3)
tc = delaunayn(pc)

# example tetramesh
library(rgl)
clr = rep(1,3) %o% (1:nrow(tc)+1)
rgl.viewpoint(60,fov=20)
rgl.light(270,60)
tetramesh(tc,pc,alpha=0.7,col=clr)

my.make.elist <- function(tc){
	k <- ncol(tc)
	r <- nrow(tc)
	cmb <- combinations(k,2)
	v1 <- v2 <- c()
	for(i in 1:nrow(cmb)){
		v1 <- c(v1,tc[,cmb[i,1]])
		v2 <- c(v2,tc[,cmb[i,2]])
	}
	el <- cbind(v1,v2)
	el <- t(apply(el,1,sort))
	unique(el)
}

el <- my.make.elist(tc)
g <- graph.edgelist(el)
E <- as.matrix(get.adjacency(g))
E <- sign(E + t(E))
E[lower.tri(E)] <- 0
#plot(g)
#el <- get.edgelist(g)
S.list <- list()
n <- nrow(E)
S.list[[1]] <- diag(rep(1,nrow(E)))
cnt <- 1
loop <- TRUE
while(loop){
	if(length(S.list[[cnt]][,1])==0){
		break
	}
	tmp <- S.list[[cnt]] %*% E
	tmp2 <- which(tmp==cnt,arr.ind=TRUE)
	if(length(tmp2[,1])>0){
		cnt <- cnt
		S.list[[cnt+1]] <- matrix(0,length(tmp2[,1]),n)
		for(i in 1:length(tmp2[,1])){
			u <- tmp2[i,1]
			v <- tmp2[i,2]
			pre <- which(S.list[[cnt]][u,]==1)
			S.list[[cnt+1]][i,c(pre,v)] <- 1
		}
	}else{
		loop <- FALSE
	}
	cnt <- cnt+1
}

tmp <- sapply(S.list,nrow)
