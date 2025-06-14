/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package gui;
import aplicacion.Armamento;
import java.util.ArrayList;
import javax.swing.table.AbstractTableModel;
import java.util.List;

/**
 *
 * @author alumnogreibd
 */
public class ModeloTablaArmamento extends AbstractTableModel {

    private ArrayList<Armamento> armas;
    
    
    public ModeloTablaArmamento(){
        this.armas = new java.util.ArrayList<>();
    }
    
    @Override
    public int getRowCount() {
        return armas.size();
    }

    @Override
    public int getColumnCount() {
        return 4;
    }
    
    @Override
    public String getColumnName(int col){
        String nombre="";

        switch (col){
            case 0: nombre= "Nº Serie"; break;
            case 1: nombre= "Tipo"; break;
            case 2: nombre= "Modelo"; break;
            case 3: nombre= "Estado"; break;
        }
        return nombre;
    }

    @Override
    public Class getColumnClass(int col){
        Class clase=null;

        switch (col){
            case 0: clase= java.lang.Integer.class; break;
            case 1: clase= java.lang.String.class; break;
            case 2: clase= java.lang.String.class; break;
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
            case 0: resultado= armas.get(row).getNumSerie(); break;
            case 1: resultado= armas.get(row).getTipo(); break;
            case 2: resultado= armas.get(row).getModelo();break;
            case 3: resultado= armas.get(row).getEstado(); break;
        }
        return resultado;
    }

    public void setFilas(ArrayList<Armamento> armamento){
        this.armas=armamento;
        fireTableDataChanged();
    }

    public Armamento obtenerArma(int i){
        return this.armas.get(i);
    }
}
