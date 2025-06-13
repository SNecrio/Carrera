datos <- c("buena", "buena", "regular", "muy buena", "mala", "buena", "buena", "regular", "muy mala",
           "buena", "muy buena", "muy buena", "regular", "buena", "mala", "buena", "muy buena",
           "muy buena", "buena", "regular")
datos
datos0 = factor(datos)
datos0
datos = factor(datos, levels = levels(datos0)[c(4,2,5,1,3)])
datos
fabs = table(datos)
fabs
fre = table(datos) / length(datos)
fre
fabscum = cumsum(fabs)
frecum = cumsum(fre)
fabscum
frecum

tabla = cbind(fabs,fre,fabscum,frecum)
tabla
barplot(table(datos),col = "purple")


library(MASS)
attach(quine)
head(quine)
barplot(table(Sex), col="pink")
barplot(table(Sex))

Tabla = table(Age,Sex)
Tabla
margin.table(Tabla,1)
new = addmargins(Tabla)
barplot(table(Age), col = 3)

attach(geyser)
head(geyser)
barplot(duration)
pie(duration)
hist(duration)
par(mfrow = c(2,2))
hist(duration, breaks = 4)
hist(duration, breaks = 9)
hist(duration, breaks = 24)
hist(duration, breaks = 49)

mean(duration)
median(duration)
quantile(duration, probs=0.7)
quantile(duration, probs = c(0.25,0.5,0.75))
summary(duration)
var(duration) #cuasi varianza
sd(duration) #desviacion tipica
range(duration)
diff(range(duration))
attach(quine)
head(quine)

table(Sex)
table(Sex,Days)
a = factor(Days)
DaysM = Days[Sex=="M"]
DaysF = Days[Sex != "M"]

par(mfrow=c(1,1))
barplot(table(DaysM)/length(DaysM))
barplot(table(DaysF)/length(DaysF))

attach(airquality)
head(airquality)
Wind
summary(Wind)

par(mfrow=c(1,2))
hist(Wind, breaks=4)
hist(Wind, breaks=9)
hist(Wind, breaks=14)
hist(Wind, breaks=29)
summary(Wind)[5] *1.5
boxplot(Wind)


setwd("C:/Users/1234/Documents/1º Grado/Estadistica/Practica1/DatosIGE.csv")

datos = read.csv("DatosIGE.csv")
attach(datos)
lugenses = datos[CodEspazo == 27,]
lug2019 = lugenses[lugenses$Tempo == "2019",]

lugH = lugenses[lugenses$Sexo=="Homes",]
lugM = lugenses[lugenses$Sexo=="Mulleres",]

lugHn = as.numeric(lugH$Idade)
lugMn = as.numeric(lugM$Idade)

summary(lugHn)
summary(lugMn)
lugHNa = na.omit(lugHn)
lugMNa = na.omit(lugMn)
boxplot(lugHNa)
boxplot(lugMNa)