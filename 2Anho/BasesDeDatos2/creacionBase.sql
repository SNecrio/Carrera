create table grupoArmado(
	id int primary key,
	nombre varchar(40) not null unique
);

create table paisGrupoArmado(
	idGrupo int,
	pais varchar(40),
	PRIMARY KEY (idGrupo, pais),
	FOREIGN KEY (idGrupo) REFERENCES grupoArmado(id) ON DELETE cascade on update cascade
); 
	
create table armamento(
	numSerie int not null,
	tipo varchar(40) not null,
	modelo varchar(40) not null,
	estado varchar(40),
	ubicacion varchar(40),
	fechaAdquisicion date not null,
	idGrupo int not null,
	primary key (numSerie, idGrupo),
	FOREIGN KEY (idGrupo) REFERENCES grupoArmado(id) ON DELETE cascade on update cascade
);

create table equipoComunicacion(
	id int not null,
	companhia varchar(40),
	nombre varchar(40) not null,
	fechaAdquisicion date not null,
	tipo varchar(40) not null,
	alcance decimal(10,2) not null,
	idGrupo int not null,
	primary key (id, idGrupo),
	FOREIGN KEY (idGrupo) REFERENCES grupoArmado(id) ON DELETE cascade on update cascade
);

create table equipoProteccion(
	id int not null,
	companhia varchar(40),
	nombre varchar(40) not null,
	fechaAdquisicion date not null,
	tipo varchar(40) not null,
	material varchar(40) not null,
	idGrupo int not null,
	primary key (id, idGrupo),
	FOREIGN KEY (idGrupo) REFERENCES grupoArmado(id) ON DELETE cascade on update cascade
);

create table equipoMedico(
	id int not null,
	companhia varchar(40),
	nombre varchar(40) not null,
	fechaAdquisicion date not null,
	tipo varchar(40) not null,
	categoria varchar(40) not null,
	idGrupo int not null,
	primary key (id, idGrupo),
	FOREIGN KEY (idGrupo) REFERENCES grupoArmado(id) ON DELETE cascade on update cascade
);

create table miembro(
	dni varchar(9) not null,
	nombre varchar(40) not null,
	rango varchar(40) not null,
	especializacion varchar(20),
	salud varchar(40) not null default 'Sano',
	idGrupo int not null,
	esLider boolean not null default false,
	clave varchar(40),
	primary key (dni, idGrupo),
	FOREIGN KEY (idGrupo) REFERENCES grupoArmado(id) ON DELETE cascade on update cascade
);

create table conflicto(
	nombre varchar(40) not null,
	fechaInicio date not null,
	fechaFin date,
	ubicacion varchar(40),
	primary key (nombre)
);

create table participaConflicto(
	nombre varchar(40) not null,
	idGrupo int not null,
	primary key(nombre, idGrupo),
	FOREIGN KEY (nombre) REFERENCES conflicto(nombre) ON DELETE cascade on update cascade,
	FOREIGN KEY (idGrupo) REFERENCES grupoArmado(id) ON DELETE cascade on update cascade
);

create table medalla(
	nombre varchar(40) not null,
	descripcion varchar(100) not null,
	nombreConflicto varchar(40) not null,
	dniMiembro varchar(9) not null,
	idGrupo int not null,
	primary key (nombre, nombreConflicto, dniMiembro, idGrupo),
	FOREIGN KEY (nombreConflicto) REFERENCES conflicto(nombre) ON DELETE cascade on update cascade,
	foreign key (dniMiembro, idGrupo) references miembro(dni, idGrupo) on delete cascade on update cascade
);

create table certificacion(
	nombre varchar(40) not null,
	descripcion varchar(100) not null,
	primary key (nombre)
);
			 
create table miembrosCertificaciones(
	dni varchar(9) not null,
	idGrupo int not null,
	certificacion varchar(40) not null,
	primary key (dni, idGrupo, certificacion),
	FOREIGN KEY (dni, idGrupo) REFERENCES miembro(dni, idGrupo) ON DELETE cascade on update cascade,
	FOREIGN KEY (certificacion) REFERENCES certificacion(nombre) ON DELETE cascade on update cascade
); 

create table mision(
	id int not null,
	nombre varchar(40) not null,
	fecha date,
	lugar varchar(40),
	bajasEquipo int,
	bajasCiviles int,
	objetivo varchar(100) not null,
	primary key(id)
);

create table cuartel(
	nombre varchar(40) not null,
	ubicacion varchar(40) not null,
	idGrupo int not null,
	primary key (nombre,idGrupo),
	FOREIGN KEY (idGrupo) REFERENCES grupoArmado(id) ON DELETE cascade on update cascade
);

create table acceder(
	hora time not null,
	fecha date not null,
	cuartel varchar(40) not null,
	idGrupo int not null,
	dni varchar(9) not null,
	primary key (hora, fecha,cuartel,dni,idGrupo),
	FOREIGN KEY (idGrupo, cuartel) REFERENCES cuartel(idGrupo, nombre) ON DELETE cascade on update cascade,
	FOREIGN KEY (dni, idGrupo) REFERENCES miembro(dni, idGrupo) ON DELETE cascade on update cascade
);

create table participarMision(
	dni varchar(9) not null,
	idGrupo int not null,
	mision int not null,
	esBaja bool default false,
	primary key(dni,mision),
	FOREIGN KEY (dni, idGrupo) REFERENCES miembro(dni, idGrupo) ON DELETE cascade on update cascade,
	FOREIGN KEY (mision) REFERENCES mision(id) ON DELETE cascade on update cascade
);
