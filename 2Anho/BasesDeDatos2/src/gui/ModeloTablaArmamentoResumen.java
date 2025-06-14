/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package gui;
import aplicacion.Armamento;
import java.util.ArrayList;
import java.util.HashMap;
import javax.swing.table.AbstractTableModel;
import java.util.List;
import java.util.Map;

/**
 *
 * @author alumnogreibd
 */
public class ModeloTablaArmamentoResumen extends AbstractTableModel {

    ArrayList<String> tipos;
    ArrayList<Integer> cantidad;
    
    
    public ModeloTablaArmamentoResumen(){
        tipos = new ArrayList<>();
        cantidad = new ArrayList<>();
    }
    
    @Override
    public int getRowCount() {
        return tipos.size();
    }

    @Override
    public int getColumnCount() {
        return 2;
    }
    
    @Override
    public String getColumnName(int col){
        String nombre="";

        switch (col){
            case 0: nombre= "Tipo"; break;
            case 1: nombre= "Cantidad"; break;
        }
        return nombre;
    }

    @Override
    public Class getColumnClass(int col){
        Class clase=null;

        switch (col){
            case 0: clase= java.lang.String.class; break;
            case 1: clase= java.lang.Integer.class; break;
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
            case 0: resultado= tipos.get(row); break;
            case 1: resultado= cantidad.get(row); break;
        }
        return resultado;
    }

    public void setFilas(HashMap<String, Integer> resu){
        tipos.clear();
        cantidad.clear();
        for (Map.Entry<String, Integer> entry : resu.entrySet()) {
            String key = entry.getKey();
            int val = entry.getValue();
            tipos.add(key);
            cantidad.add(val);
        }
        fireTableDataChanged();
    }
    

    //public Armamento obtenerArma(int i){
        //return this.armas.get(i);
    //}
}
