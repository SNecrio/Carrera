#El soporte
x <- 1:6
#La masa de probabilidad
px <- seq(11,1,by = -2)/36
sum(px)
#La funcion de distribcion
Fx = cumsum(px)
#Barplot
barplot(px,names = x,xlab = 'x',ylab = 'P(X=x)', main ='Masa de probabilidade',ylim=c(0,0.35))
#Masa de probabilidad
plot(x,px,type='h',xlim = c(0.5,6.5),ylim=c(0,0.35),main = 'Masa de probabilidade',
     xlab='x',ylab='P(X=x)')
#Funcion de distribucion
plot(x,Fx,type='s',xlim = c(0.5,6.5),ylim=c(-0.1,1.1),main = 'Funcion de distribucion',
     xlab='x',ylab='F(x)')
#Añadimos segmentos para completarla
segments(0.5,0,1,0)
segments(1,0,1,Fx[1])
segments(6,1,6.5,1)

#Esperanza de la variable aleatoria
Ex = sum(x*px)
Ex
weighted.mean(x,px)
#Varianza
Vx = sum((x-Ex)^2*px)
sum(x^2*px) - Ex^2

#La funcion de densidad
fdens = function(x){
  return(0.75*(1-x^2))
}
#Aproximamos a integral
integrate(fdens,-1,1)
#A funcion de distribucion
Fdist = function(x){
  -x^3/4+3/4*x+1/2
}
#Funcion de densidad
x = seq(-1,1, by = 0.01)
plot(x,fdens(x),type='l',xlim = c(-1.5,1.5),ylim=c(-0.05,0.8),main ='Funcion de densidad',
     xlab='x',ylab='f(x)')
segments(-1.5,0,-1,0)
segments(1,0,1.5,0)
#Funcion de distribucion
plot(x,Fdist(x),type='l',xlim = c(-1.5,1.5),ylim=c(-0.05,1.05),
     main ='Funcion de distribucion',
     xlab='x',ylab='f(x)')
segments(-1.5,0,-1,0)
segments(1,1,1.5,1)


#La probabilidad de que -0.5 < X < 1
integrate(fdens,-0.5,1)
Fdist(1) - Fdist(-0.5)
#Calculamos la media con integrate
f_med = function(x){
  return(x*fdens(x))
}
Ex = integrate(f_med,-1,1)
Ex
names(Ex)
#Solo nos interesa la integral
Ex = Ex$value
Ex

f_var = function(x){
  return((x-Ex)^2*fdens(x))
}


rbinom(5,6,0.3)
#Calculamos la probabilidad de que x <= 4
pbinom(4,6,0.3)
sum(dbinom(0:4,6,0.3))
dbinom(0:4,6,0.3)

sum(dbinom(3:5,6,0.3))
pbinom(5,6,0.3) - pbinom(2,6,0.3)
#Genera valores
rnbinom(4,8,0.6)

#Numero de errores antes del primer acierto = X
#size = 1
#prob = 0.65

#X = 3
dnbinom(3,1,0.65)
#X <= 3
pnbinom(3,1,0.65)
sum(dnbinom(0:3,1,0.65))

#Poisson, lambda = 2
#X >= 4
1- ppois(3,2)

#SQUID GAME
#X = errores en el juego A
#Y = errores en el juego B

winA = dbinom(0,3,0.5)

winB = pbinom(2,6,0.5)
#60% de que toque el juego B
0.4 * winA + 0.6 * winB

9/47 + 9/46

#X = numero de treboles en las cartas pro salir
#X sigue una distribucion hipergeometrica
#m = numero de treboles en la baraja
#m = 13 - 4 = 9
#n = el resto de cartas
#n = 52 - 5 - 9 = 38
#k = cartas que se sacan
#k = 2

#P(X <= 2)

pcolor = 1- dhyper(0,9,38,2)
sum(dhyper(1:2,9,38,2))

#X = numero de treboles en las cartas pro salir
#X sigue una distribucion hipergeometrica
#m = numero de treboles en la baraja
#m = 13 - 2 = 11
#n = el resto de cartas
#n = 52 - 2 - 11 = 39
#k = cartas que se sacan
#k = 5
#P(X >= 3)

1 - phyper(2,11,39,5)
sum(dhyper(3:5,11,39,5))

#10 valores entre 0 e 1
runif(10,0,1)

runif(10)
runif(15,3,4)

#Normal
#Media = 160
#Desviacion tipica = 10

#P(X <= 150)
pnorm(150,160,10)
#P(X <= 160)
pnorm(160,160,10)

#Generamos 100 valores de la normal N(1,4)
datos = rnorm(100,1,2)
#Histograma
hist(datos,freq = F, main = '100 Observaciones de una N(1,4',xlab='x',ylab='Densidade')
x = seq(min(datos),max(datos),by=0.01)
fx = dnorm(x,1,2)
lines(x,fx,col = 'red')

x = seq(-4,4,by =0.01)
plot(x, dnorm(x),type='l',ylim=c(0,1))
lines(x,dnorm(x,0,2),col=2)
lines(x,dnorm(x,0,0.5),col=3)
#Mediana de una normal N(0,1)
qnorm(0.5)

qnorm(0.05)
#Cuantil 0,8 de la N(160,64)
qnorm(0.8,160,8)
#Plot Bi (40,0.25)
plot(0:40, dbinom(0:40,40,0.25), type='h')

mu = 40*0.25
sigma2 = 40*0.25 * (1-0.25)
x = seq(0,40,by = 0.1)
lines(x,dnorm(x,mu,sqrt(sigma2)))
