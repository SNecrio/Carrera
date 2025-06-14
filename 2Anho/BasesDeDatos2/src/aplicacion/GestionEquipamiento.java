/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package aplicacion;
import gui.FachadaGui;
import baseDatos.FachadaBaseDatos;
import java.util.*;
/**
 *
 * @author alumnogreibd
 */
public class GestionEquipamiento {
    
    FachadaGui fgui;
    FachadaBaseDatos fbd;
    
    public GestionEquipamiento(FachadaGui fgui, FachadaBaseDatos fbd){
        this.fgui=fgui;
        this.fbd=fbd;
    }
    
    public ArrayList<Armamento> obtenerArmamento(String serie, String modelo, String tipo){
        return fbd.obtenerArmamento(serie, modelo, tipo);
    }
    
    public void insertarArmamento(Armamento arma){
        fbd.insertarArmamento(arma);
    }
    
    public void borrarArmamento(int i){
        fbd.borrarArmamento(i);
    }
    
    public void actualizarArmamento(int i, Armamento arma){
        fbd.actualizarArmamento(i, arma);
    }
    
    public void armamentoSetGrupo(int id){
        fbd.armamentoSetGrupo(id);
    }
  
    
    public ArrayList<Equipo> obtenerEquipo(String id, String nombre, int tipo){
        return fbd.obtenerEquipo(id, nombre, tipo);
    }
    
    public void insertarEquipo(Equipo equipo, int tipo){
        fbd.insertarEquipo(equipo, tipo);
    }
    
    public void borrarEquipo(int i, int tipo){
        fbd.borrarEquipo(i, tipo);
    }
    
    public void actualizarEquipo(int i, Equipo equipo, int tipo){
        fbd.actualizarEquipo(i, equipo, tipo);
    }
    
    public void equipoSetGrupo(int id){
        fbd.equipoSetGrupo(id);
    }
    
    public HashMap<String, Integer> obtenerResumen(){
        return fbd.obtenerResumen();
    }
}
