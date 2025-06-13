getwd()
setwd("C:/Users/1234/Documents/1º Grado/Estadistica/Práctica 6")

#Session -> set working directory -> choose directory

datos <- read.csv("datos_practica6.csv")
head(datos)

datos1<-datos$x1

# IC para mu al 90% sabiendo que varianza conocida = 2

# Sin necesidad de comandos, podemos hacerlo usando la fórmula

# X BARRA
xbarra <- mean(datos1)
xbarra

# CUANTIL Z_(1-alfa/2)
# 1-alfa=0.90 -> alfa = 0.1      Z_(1-0.1/2) -> Z_0.95
cuantil <- qnorm(0.95,0,1)
cuantil

# SIGMA -> desviación típica poblacional
sigma <- sqrt(2)

# Tamaño muestral
n<-length(datos1)
n


# Límite inferior
LI <- xbarra-cuantil*sigma/sqrt(n)
LI

# Límite superior
LS <- xbarra+cuantil*sigma/sqrt(n)
LS

# IC para mu al 90%
c(LI,LS)

install.packages("LearningStats")
library(LearningStats)  # warning porque construido en version anterior

Mean.CI(datos1, sigma=sqrt(2), conf.level=0.9)  #Confidence interval

# APARTADO 2

datos2<-datos$x2

# Calcula IC para mu al 95%
# No podemos copiar lo que hicimos nates poque ahora no conocemos la varianza

xbarra<-mean(datos2)
xbarra

cuantil<-qt(0.975, df=(length(datos2)-1))
cuantil

# Cuasi-desviación típica muestral
Sc <- sd(datos2)
Sc
# lo mismo que hacer sqrt(var)

# Límite inferior
LI <- xbarra-cuantil*Sc/sqrt(length(datos2))
LI

# Límite superior
LS <- xbarra+cuantil*Sc/sqrt(length(datos2))
LS

# IC para mu al 90%
c(LI,LS)

# También como
Mean.CI(datos2, conf.level=0.95)  #Confidence interval

t.test(datos2)
t.test(datos2,conf.level = 0.95)

# CONTRASTE BILATERAL
# H0: mu=4
# H1: mu!=4
# Nivel de significacion alfa=0.05
t.test(datos2,mu=2)

# CONTRASTE UNILATERAL ESQUERDO
t.test(datos2,mu=4,alternative="greater")

# CONTRASTE UNILATERAL DEREITO
t.test(datos2,mu=4,alternative="less")

# Apartado c)
datos3<-datos$x3

n<-length(datos3)

Sc2<-var(datos3)

# Cuantil de una chi-cuad con (n-1) grados de libertad: (1-alfa/2)=0.985

cuantil1<-qchisq(0.985, df=n-1)

cuantil2<-qchisq(0.015,df=n-1)

# Límite inferior
LI <- (n-1)*Sc2/cuantil1
LI

# Límite superior
LS <- (n-1)*Sc2/cuantil2
LS

# IC para sigma^2 al 97%
c(LI,LS)

variance.CI(datos3, conf.level = 0.97)

# EJERCICIO 2

# Compré 200, salieron 9 defectuosos
# Estimar p?
pgorro<-9/200
pgorro

# IC para la verdadera proporción de productos defectuosos al 99%

# Opción 1: a mano
# pgorro+- Z_(1-alfa/2)*sqrt((pgorro*(a-pgorro))/m)

# Opción 2:
proportion.CI(9,200, conf.level = 0.99)

# Opción 3:
prop.test(9,200, conf.level=0.99)
