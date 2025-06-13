?cos

#Funciones, librerias, carga de datos externos -> Practica 1
#Hacer tabla de frecuencias y plots -> Practica 2
#Mas plots box plots y demas -> Practica 3
#Generar muestras e histogramas -> Practica 5
#Intervalo de confianza -> Practica 6

#Calcular minimo y maximo de vectores
a = c(1,4,7,1,4,6)
min(a)
max(a)
length(a)
prod(a)
sum(a)

seq(0, 0.75, by = 0.25)         # Indicando la distancia entre dos valores
seq(0, 0.75, length.out = 4)    # Indicando la longitud final del vector
#   - Con dos puntos:
(0:3)/4
sort(t, decreasing=TRUE)

# Creamos el vector con la esperanza de vida de cada pais
esp.vida <- c(83.2, 82.5, 83.0, 83.4, 82.4, 81.4, 81.4, 81.8, 81.6)
# y le damos nombre a cada observacion
names(esp.vida) <- c("Espanha", "Francia", "Italia", "Suiza", "Luxemburgo","Belgica", "Reino Unido", "Irlanda", "Portugal")

mean(esp.vida)

A = matrix(1:16, nrow = 4, ncol = 4, byrow = TRUE)
B = matrix(1:16, nrow = 4, ncol = 4)
t(A)
C <- A%*%B
C

# La inversa se calcula con solve()
solve(C)
# Calculamos el determinante con det()
det(C)
# Con eigen() calculamos los autovalores y los autovetores de la matriz
eigen(C)
# Solo nos interesan los autovectores
autoval <- eigen(C)$values


#-------------------ESTADISTICOS-------------------

mean(duration) #media
median(duration) #mediana
quantile(duration,probs=0.7) #cuantil 0.7
quantile(duration, probs = c(0.25,0.5,0.75)) #cuartiles
summary(duration)
var(duration) #cuasi-
varianza=var(PCs)*(n-1)/n
sd(duration)
range(duration)
diff(range(duration)) #rango

quantile(erupciones,0.85)

#-------------------PLOTS-------------------
hectareas <- c(0.8, 1.1, 0.6, 3.2, 1.4, 2.1, 1.5, 0.2, 1.0, 6.1)
ocupacion <- c(1200, 650, 1060, 235, 1300, 300, 750, 150, 440, 500)
plot(hectareas, ocupacion)
abline(h = mean(ocupacion), col = 'red')
abline(v = mean(hectareas), col = 'blue')
#calcular coeficiente de correlación
cor(erupciones,tiempo)

# Histograma-----------

library(MASS)
attach(geyser)
names(geyser)
length(duration)

barplot(duration)
pie(duration)
?hist
hist(duration, main="Histograma del tiempo de erupción", 
     xlab="tiempo")
hist(duration, main="Histograma del tiempo de erupción", 
     xlab="tiempo",
     freq=F)

par(mfrow=c(2,2))
hist(duration,breaks=4,col="grey",main="Histograma con 5 intervalos")
hist(duration,breaks=9,col="grey",main="Histograma con 10 intervalos")
hist(duration,breaks=24,col="grey",main="Histograma con 25 intervalos")
hist(duration,breaks=49,col="grey",main="Histograma con 50 intervalos")

boxplot(duration)

#------------------DISTRIBUCIONES------------------

#P(X < 5) 
pnbinom(5,1,0.5)

#c) una erupcion es "corta" si dura menos de 3 minutos, "larga" si dura más de 4 minutos y "media" en otro caso.
# calcular la tabla de frecuencias absolutas y relativas

datos = faithful
head(datos)
erup = datos$eruptions
len = length(erup)
vector = numeric(len)

for (i in 1:len){
  if(erup[i] < 3){
    vector[i] = "corta"
  }else if(erup[i] > 4){
    vector[i] = "larga"
  }else{
    vector[i] = "media"
  }
}

vector = factor(vector, levels=c("corta","media","larga"))
vector
Fabs<-table(vector);Fabs
Frel<-table(vector)/length(vector);Frel
FabsAcum<-cumsum(Fabs)
FrelAcum<-cumsum(Frel)

Tabla<-cbind(Fabs,Frel,FabsAcum,FrelAcum);Tabla
barplot(table(datos),col=4,main="Diagrama de barras")


