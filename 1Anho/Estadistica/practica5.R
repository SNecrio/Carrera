#Numero de muestras
nrep = 500

#Tamaño muestral
n = 100

#Generamos 1 muestra de la N(160,64)
sqrt(64)
datos = rnorm(100,160,8)
datos

#Generamos 500 muestras

#Creamos un vector

est = numeric()
mu = 160
sigma = 8

for(i in 1:nrep){
  datos = rnorm(100,160,8)
  xbarra = mean(datos)
  
  est[i] = (xbarra-mu)/(sigma/sqrt(n))
}

hist(est, freq=F)
#freq hace que sea la funcion de densidad e integre a 1

#Funcion de densidad de la N(0,1)
#f(0)
dnorm(0,0,1)

#Funcion de densidad de la N(0,1) entera
seqx = seq(-4,4,len = 1000)
fdens = dnorm(seqx)
lines(seqx, fdens, col = "purple")

#EJERCICIO 2

#Numero de meustras
nrep = 500

#Tamaño muestral
n = 10

#500 muestras de tamaño 10 de una N(3,0.5)

#Vector de estadisticos
est = numeric()
sigma = 0.5
n = 10

for(i in 1:nrep){
  datos = rnorm(n,mean=3,sd=0.5)
  #Cuasi varianza
  Sc2 = var(datos)
  est[i] = (n-1)*Sc2/sigma^2
}

#Histograma
hist(est,freq = F)

#uncion de densidad de la chi-cuadrado
#con (n-1) grados de libertad

seqx = seq(0,30,len=1000)
fdens = dchisq(seqx,df=n-1)
lines(seqx,fdens,col="blue")

#EJERCICIO 3

#Generar muestra de tamaño n= 100
#de una t de Student con 5 grados de libertad
n = 100
#Genero datos de x sigue N(0,1)
datx = rnorm(100)
#Genero datos de y sigue Chi-cuad con
#5 grados de libertad
daty = rchisq(n,df=5)
#Genero datos de t de student
#con 5 grados de lib
datt = datx/sqrt(daty/5)
hist(datt,freq=F)


#funcion de la densidad de la t con 5 gr de lib

seqx = seq(-4,4,len=1000)
fdens = dt(seqx,df=5)
lines(seqx,fdens,col="blue")


#Ejercicio 4

nrep = 500

#Tamaño muestral
n = 20

#Generamos 500 muestras

#Creamos un vector

est = numeric()
mu = 5
sigma = 8

for(i in 1:nrep){
  datos = rnorm(n,5,1)
  xbarra = mean(datos)
  #Cuasi desviacion típica
  Sc = sd(datos)
  est[i] = (xbarra-mu)/(Sc/sqrt(n))
}

hist(est, freq=F)

seqx = seq(-6,6,len=1000)
fdens=dt(seqx,df=n-1)
lines(seqx,fdens,col="purple")