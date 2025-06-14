/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package aplicacion;

/**
 *
 * @author alumnogreibd
 */
import java.util.Date;

public class Armamento {

    private int numSerie;
    private String tipo;
    private String modelo;
    private String estado;
    private String ubicacion;
    private Date fechaAdquisicion;
    private int idGrupo;

    public Armamento(int numSerie, String tipo, String modelo, Date fechaAdquisicion, int idGrupo) {
        this.numSerie = numSerie;
        this.tipo = tipo;
        this.modelo = modelo;
        this.fechaAdquisicion = fechaAdquisicion;
        this.idGrupo = idGrupo;
    }

    public Armamento(int numSerie, String tipo, String modelo, String estado, String ubicacion, Date fechaAdquisicion, int idGrupo) {
        this.numSerie = numSerie;
        this.tipo = tipo;
        this.modelo = modelo;
        this.estado = estado;
        this.ubicacion = ubicacion;
        this.fechaAdquisicion = fechaAdquisicion;
        this.idGrupo = idGrupo;
    }
    

    public int getNumSerie() {
        return numSerie;
    }

    public void setNumSerie(int numSerie) {
        this.numSerie = numSerie;
    }

    public String getTipo() {
        return tipo;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

    public String getModelo() {
        return modelo;
    }

    public void setModelo(String modelo) {
        this.modelo = modelo;
    }

    public String getEstado() {
        return estado;
    }

    public void setEstado(String estado) {
        this.estado = estado;
    }

    public String getUbicacion() {
        return ubicacion;
    }

    public void setUbicacion(String ubicacion) {
        this.ubicacion = ubicacion;
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
}

