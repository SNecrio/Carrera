/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package aplicacion;
import java.util.*;


/**
 *
 * @author alumnogreibd
 */
import java.util.Date;

public class Equipo {

    private int id;
    private String companhia;
    private String nombre;
    private String tipo;
    private Date fechaAdquisicion;
    private int idGrupo;
    private String propiedad;
    
    public static final int COMUNICACION = 0;
    public static final int MEDICO = 1;
    public static final int PROTECCION = 2;

    public Equipo(int id, String companhia, String nombre, String tipo, Date fechaAdquisicion, int idGrupo, String propiedad) {
        this.id = id;
        this.companhia = companhia;
        this.nombre = nombre;
        this.tipo = tipo;
        this.fechaAdquisicion = fechaAdquisicion;
        this.idGrupo = idGrupo;
        this.propiedad = propiedad;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getCompanhia() {
        return companhia;
    }

    public void setCompanhia(String companhia) {
        this.companhia = companhia;
    }

    public String getNombre() {
        return nombre;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public String getTipo() {
        return tipo;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

    public Date getFechaAdquisicion() {
        return fechaAdquisicion;
    }

    public void setFechaAdquisicion(Date fechaAdquisicion) {
        this.fechaAdquisicion = fechaAdquisicion;
    }

    public int getIdGrupo() {
        return idGrupo;
    }

    public void setIdGrupo(int idGrupo) {
        this.idGrupo = idGrupo;
    }

    public String getPropiedad() {
        return propiedad;
    }

    public void setPropiedad(String propiedad) {
        this.propiedad = propiedad;
    }
}

