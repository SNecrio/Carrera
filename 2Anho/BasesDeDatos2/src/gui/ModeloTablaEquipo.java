/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package gui;
import aplicacion.Equipo;
import java.util.ArrayList;
import javax.swing.table.AbstractTableModel;

/**
 *
 * @author alumnogreibd
 */
public class ModeloTablaEquipo extends AbstractTableModel {

    private ArrayList<Equipo> equipos;
    private  int modo = 0;
    
    public ModeloTablaEquipo(){
        this.equipos = new java.util.ArrayList<>();
    }
    
    @Override
    public int getRowCount() {
        return equipos.size();
    }

    @Override
    public int getColumnCount() {
        return 4;
    }
    
    @Override
    public String getColumnName(int col){
        String nombre="";

        switch (col){
            case 0: nombre= "Nombre"; break;
            case 1: nombre= "Tipo"; break;
            case 2: nombre= "F. Adquisicion"; break;
            case 3:
                switch(modo){
                    case Equipo.COMUNICACION: nombre = "Caracteristica";
                    case Equipo.MEDICO: nombre = "Caracteristica";
                    case Equipo.PROTECCION: nombre = "Caracteristica";
                }; break;
        }
        return nombre;
    }

    @Override
    public Class getColumnClass(int col){
        Class clase=null;

        switch (col){
            case 0: clase= java.lang.String.class; break;
            case 1: clase= java.lang.String.class; break;
            case 2: clase= java.util.Date.class; break;
            case 3: clase= java.lang.String.class; break;
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
            case 0: resultado= equipos.get(row).getNombre(); break;
            case 1: resultado= equipos.get(row).getTipo(); break;
            case 2: resultado= equipos.get(row).getFechaAdquisicion();break;
            case 3: resultado= equipos.get(row).getPropiedad(); break;
        }
        return resultado;
    }

    public void setFilas(ArrayList<Equipo> equipo){
        this.equipos=equipo;
        fireTableDataChanged();
    }

    public Equipo obtenerEquipo(int i){
        return this.equipos.get(i);
    }
    
    public void setModo(int i){
        if(i >= 0 && i <= 2){
            modo = i;
        }
    }
}
