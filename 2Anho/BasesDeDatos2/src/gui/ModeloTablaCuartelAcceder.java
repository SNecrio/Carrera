/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package gui;
import javax.swing.table.*;
import java.util.*;
import aplicacion.Cuartel;
/**
 *
 * @author alumnogreibd
 */
public class ModeloTablaCuartelAcceder extends AbstractTableModel{
    
    private ArrayList<Cuartel> cuarteles;
    
    public ModeloTablaCuartelAcceder(){
        this.cuarteles = new ArrayList<>();
    }
    
    @Override
    public int getRowCount() {
        return cuarteles.size();
    }

    @Override
    public int getColumnCount() {
        return 2;
    }
    
     @Override
    public String getColumnName(int col){
        String nombre="";

        switch(col){
            case 0: nombre= "Nombre"; break;
            case 1: nombre= "Ubicacion"; break; 
        }
        return nombre;
    }
    
    @Override
    public Class getColumnClass(int col){
        Class clase=null;

        switch (col){
            case 0: clase= java.lang.String.class; break;
            case 1: clase= java.lang.String.class; break;
        }
        return clase;
    }
    
    @Override
    public boolean isCellEditable(int row, int col){
        return false;
    }
    
    @Override
    public Object getValueAt(int row, int col){
        Object resultado=null;

        switch (col){
            case 0: resultado= cuarteles.get(row).getNombre(); break;
            case 1: resultado= cuarteles.get(row).getUbicacion(); break;
        }
        return resultado;
    }
    
    
    public void setFilas(ArrayList<Cuartel> cuarteles){
        this.cuarteles=cuarteles;
        fireTableDataChanged();
    }
    
    public Cuartel obtenerCuarteles(int i){
        return this.cuarteles.get(i);
    }
    
    public Cuartel obtenerCuartel(int i){
        return this.cuarteles.get(i);
    }
    
    public void eliminarCuartel(int i){
        this.cuarteles.remove(i);
        fireTableRowsDeleted(i, i);
    }
    
    public void anhadirCuartel(Cuartel cuartel){
        this.cuarteles.add(cuartel);
        fireTableRowsInserted(cuarteles.size()-1, cuarteles.size()-1);
    }
    
    public void modificarCuartel(int i, Cuartel cuartelModificado){
        this.cuarteles.set(i, cuartelModificado);
        fireTableRowsUpdated(i, i);
    }
}
