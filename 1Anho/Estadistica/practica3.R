datos <- read.csv("C:/Users/1234/Documents/1º Grado/Estadistica/Practica3/DatosGapminder.csv")

head(datos)
attach(datos)

boxplot(PCs,main = "Boxplot",col="purple")
hist(PCs,main="Histograma",col="purple")
?var
n = length(PCs)
varianza=var(PCs)*(n-1)/n
desvtip=sqrt(varianza)

prob=c(0.3,0.6,0.9)
quantile(PCs,prob)

install.packages("moments")
library(moments)
skewness(PCs)

??skewness

lambda=seq(-2,2,by=0.01)
coef.simetria=numeric(length(lambda))

for(i in 1:length(lambda)){
  if(lambda[i]==0){
    tbx=log(PCs)
    coef.simetria[i]=skewness(tbx)
  }else{
    tbx=(PCs^lambda[i]-1)/lambda[i]
    coef.simetria[i] = skewness(tbx)
  }
}

plot(lambda,abs(coef.simetria))
min(abs(coef.simetria))

which.min(abs(coef.simetria))
lambda[which.min(abs(coef.simetria))]

tPCsnew=(PCs^0.73-1)/0.73
boxplot(tPCsnew)

n=length(PCs)
media_PCs=mean(PCs)
var_PCs=var(PCs)*(n-1)/n
PCs_tipi=(PCs-media_PCs)/sqrt(var_PCs)

boxplot(PCs_tipi)

#

GDP
boxplot(GDP)
hist(GDP)
ng = length(GDP)
varianzag=var(GDP)*(ng-1)/ng
desvtipg=sqrt(varianzag)

quantile(GDP,prob)
skewness(GDP)

for(i in 1:length(lambda)){
  if(lambda[i]==0){
    tbxg=log(GDP)
    coef.simetria[i]=skewness(tbxg)
  }else{
    tbxg=(GDP^lambda[i]-1)/lambda[i]
    coef.simetria[i] = skewness(tbxg)
  }
}

plot(lambda,abs(coef.simetria))
min(abs(coef.simetria))

which.min(abs(coef.simetria))
lambda[which.min(abs(coef.simetria))]

tGDPnew=(GDP^0.12-1)/0.12
boxplot(tGDPnew)

n=length(GDP)
media_GDP=mean(GDP)
var_GDP=var(GDP)*(ng-1)/ng
GDP_tipi=(GDP-media_GDP)/sqrt(var_GDP)

boxplot(GDP_tipi)

#2

plot(PCs,GDP)

cov(PCs,GDP)
cor(PCs,GDP)
cov(PCs,GDP)/(sd(PCs)*sd(GDP))

modelo = lm(GDP~PCs)
modelo

beta0=modelo$coefficients[1]
beta1=modelo$coefficients[2]

plot(PCs,GDP)
abline(beta0,beta1,col="purple",lwd=2)
legend(x="topleft",legend="Ajuste lineal",lwd=2,col="purple")

summary(modelo)
(cor(PCs,GDP))^2

residuos = modelo$residual
plot(PCs,residuos)
abline(h=0)

x0=c(5,25,75)
pred=beta0 + beta1*x0
pred

x0 = data.frame(PCs = c(5,25,75))
predict(modelo,x0)


UE=c("Austria", "Belgium", "Bulgaria", "Croatia", "Republic of Cyprus",
     "Czech Republic", "Denmark", "Estonia", "Finland", "France", "Germany",
     "Greece", "Hungary", "Ireland", "Italy", "Latvia", "Lithuania",
     "Luxembourg", "Malta", "Netherlands", "Poland", "Portugal", "Romania",
     "Slovakia", "Slovenia", "Spain", "Sweden")
pp = datos$Paises[datos$Paises %in% UE]
pos= which(datos$Paises %in% UE)
PCs_UE = PCs[pos]
GDP_UE = GDP[pos]
modUE = lm(GDP_UE~PCs_UE)

summary(modUE)

