#!/bin/bash

#Comprobar que se pasou unha data como argumento
if [ $# -eq 0 ]; then
	echo "Por favor, introduza unha data (dd/mm/aa) e una hora (h:min:s)"
	exit 1
fi

# Obter a data pasada como argumento
fechaDada=$1
# Obter a data actual
fecha=$(date +"%d/%m/%Y %H:%M:%S")

# Usamos IFS (Internal Field Separator) para separar la fecha y la hora
IFS=' ' read -r fechaParte horaParte <<< "$fecha" #Primeiro separamos a cadea en data e hora
IFS='/' read -r d m a <<< "$fechaParte"
IFS=':' read -r h min s <<< "$horaParte"

IFS='/' read -r dDada mDada aDada <<< "$fechaDada"
if [ $# -eq 2 ]; then
	horaDada=$2
   	IFS=':' read -r hDada minDada sDada <<< "$horaDada"
else #Se non se da hora, tómase o inicio do día
	hDada="00"
	minDada="00"
	sDada="00"
fi

#Imprime as datas
echo "Fecha actual:"
echo "--------------"
echo "$d/$m/$a $h:$min:$s"
echo "--------------"
echo " "
echo "Fecha dada:"
echo "--------------"
echo "$dDada/$mDada/$aDada $hDada:$minDada:$sDada"
echo "--------------"
echo ""

#Pasamos os número a decimalíííññññ
d=$((10#$d))
m=$((10#$m))
h=$((10#$h))
min=$((10#$min))
s=$((10#$s))

dDada=$((10#$dDada))
mDada=$((10#$mDada))
hDada=$((10#$hDada))
minDada=$((10#$minDada))
sDada=$((10#$sDada))

#Establecemos algunhas variables para o calculo de dias
meses=(31 28 31 30 31 30 31 31 30 31 30 31)
numero=0
i=0
#Calculamos os días totais para a data actual
#Se o ano é bisiesto, febreiro ten un día máis
if (( (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0) )); 
then 
	(( meses[1]=29 )) 
fi
#Sumamos os días de todos os meses ata o actual
for (( i=0; i<m-1; i++ )); do
    (( numero += meses[i] ))
done
#Sumamos os días individuais
(( numero=numero+d ))
#Sumamos os días correspondentes aos anos anteriores
(( numero=numero+((a-1)*365) ))
#Sumamos os dís extra de todos os anos bisiestos anteriores
for (( i=0;i<a;i++ )) do
	if (( (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0) )); then 
		(( numero+=1 ))
	fi
done

#Repetimos o procedementos para a data dada
meses=(31 28 31 30 31 30 31 31 30 31 30 31)
numero2=0

if (( (aDada % 4 == 0 && aDada % 100 != 0) || (aDada % 400 == 0) )); then 
	(( meses[1]=29 ))
fi
#Sumamos os días de todos os meses ata o actual
for (( i=0; i<mDada-1; i++ )); do
    (( numero2 += meses[i] ))
done
#Sumamos os días individuais
(( numero2=numero2+dDada ))
#Sumamos os días correspondentes aos anos anteriores
(( numero2=numero2+((aDada-1)*365) ))
#Sumamos os dís extra de todos os anos bisiestos anteriores
for (( i=0;i<aDada;i++ )) do
	if (( (aDada % 4 == 0 && aDada % 100 != 0) || (aDada % 400 == 0) )); then 
		(( numero2+=1 ))
	fi
done

#Temos en conta o cambio ao calendario gregoriano, restando 10 días se a data é inferior ao día do cambio
if (( (aDada==1582) )); then
	if (( mDada==10 )); then
		if (( dDada>3 && dDada<16 )); then
			echo "Por el cambio de calendario, el día dado no ocurrió"
			exit 1
		elif (( dDada<4 )); then
			(( numero2-=10 ))
		fi
	elif (( mDada < 10 )); then
		(( numero2-=10 ))
	fi
	
elif (( (aDada<1582) )); then
	(( numero2-=10 ))
fi

#Calculamos o número de días de diferencia entre as dúas datas
diferencia=0
if ((numero>numero2)); then
	(( diferencia=numero-numero2 ))
else
	(( diferencia=numero2-numero ))
fi

#Calculamos as horas de diferencia entre as dadas en segundos
((hora1=h*3600+min*60+s))
((hora2=hDada*3600+minDada*60+sDada))

if (( hora1>=hora2 )); then
	(( hora1=hora1-hora2 ))
else
	(( hora1=hora2-hora1 ))
    	(( diferencia-=1 )) #Se a hora dada é superior, hai un día menos de diferencia ao calculado
fi

#Calcula os anos e días de diferencia
((anos=diferencia/365))
((dias=diferencia%365))
#Pasa o tempo, previamente en segundos, a minutos
((hora1=hora1/60))
# Imprimimos os resultados
echo "Anos de separación: $anos"
echo "Días de separación: $dias"
echo "Minutos de separación: $hora1"


