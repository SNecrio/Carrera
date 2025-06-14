/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package baseDatos;
import aplicacion.Equipo;
import java.sql.*;
import java.util.*;
/**
 *
 * @author alumnogreibd
 */
public class DAOEquipo extends AbstractDAO {
    
    private int idGrupo;
    
    public DAOEquipo(Connection conexion, aplicacion.FachadaAplicacion fa){
        super.setConexion(conexion);
        super.setFachadaAplicacion(fa);
        
    }
    
    public ArrayList<Equipo> obtenerEquipo(String id, String nombre, int tipo){
        ArrayList<Equipo> resultado = new ArrayList<>();
        Equipo equipoActual;
        
        Connection con = this.getConexion();
        PreparedStatement stmCatalogo = null;
        ResultSet rsCatalogo = null;

        StringBuilder consulta = new StringBuilder("SELECT * FROM (" +
            "SELECT id, companhia, nombre, fechaAdquisicion, tipo, idGrupo, 'C' AS origen, CAST(alcance AS VARCHAR) AS especifico FROM equipoComunicacion UNION ALL " +
            "SELECT id, companhia, nombre, fechaAdquisicion, tipo, idGrupo, 'M' AS origen, categoria AS especifico FROM equipoMedico UNION ALL " +
            "SELECT id, companhia, nombre, fechaAdquisicion, tipo, idGrupo, 'P' AS origen, material AS especifico FROM equipoProteccion) AS todas");

        switch(tipo){
            case Equipo.COMUNICACION:
                consulta.append( " WHERE origen = 'C'");
                break;
            case Equipo.MEDICO:
                consulta.append( " WHERE origen = 'M'");
                break;
            case Equipo.PROTECCION:
                consulta.append( " WHERE origen = 'P'");
                break;
        }
        
        
        if(id != null && !id.isEmpty()){
            consulta.append( " AND id = ?");
        }
        
        if(nombre != null && !nombre.isEmpty()){
            consulta.append(" AND nombre LIKE ?");
        }
        
        consulta.append(" AND idGrupo = ?");
        
        System.out.println("Consulta SQL: " + consulta.toString());

       try {
            stmCatalogo = con.prepareStatement(consulta.toString());

            int paramIndex = 1;

            if (id != null && !id.isEmpty()) {
                stmCatalogo.setInt(paramIndex++, Integer.parseInt(id));
            }
            if (nombre != null && !nombre.isEmpty()) {
                stmCatalogo.setString(paramIndex++, "%" + nombre + "%");
            }
            
            
            stmCatalogo.setInt(paramIndex++, idGrupo);

            rsCatalogo = stmCatalogo.executeQuery();
            
            while (rsCatalogo.next()) {
                equipoActual = new Equipo(rsCatalogo.getInt("id"), rsCatalogo.getString("companhia"),
                        rsCatalogo.getString("nombre"), rsCatalogo.getString("tipo"), rsCatalogo.getDate("fechaAdquisicion"), 
                        rsCatalogo.getInt("idGrupo"), rsCatalogo.getString("especifico"));

                resultado.add(equipoActual);
            }

        } catch (SQLException e) {
            System.out.println("Error al obtener equipo: " + e.getMessage());
        } finally {
            try {
                if (rsCatalogo != null) rsCatalogo.close();
                if (stmCatalogo != null) stmCatalogo.close();
            } catch (SQLException ex) {
                System.out.println("Error cerrando recursos: " + ex.getMessage());
            }
        }

        return resultado;
    }
    
    public void insertarEquipo(Equipo equipo, int tipo){
        
        Connection con = this.getConexion();
        PreparedStatement stmt = null;

        StringBuilder consulta = new StringBuilder("INSERT INTO equipo");

        switch(tipo){
            case Equipo.COMUNICACION:
                consulta.append( "Comunicacion(id,companhia,nombre,fechaAdquisicion,tipo,idGrupo,alcance) VALUES(?,?,?,?,?,?,?)");
                break;
            case Equipo.MEDICO:
                consulta.append( "Medico(id,companhia,nombre,fechaAdquisicion,tipo,idGrupo,categoria) VALUES(?,?,?,?,?,?,?)");
                break;
            case Equipo.PROTECCION:
                consulta.append( "Proteccion(id,companhia,nombre,fechaAdquisicion,tipo,idGrupo,material) VALUES(?,?,?,?,?,?,?)");
                break;
        }
        
        try{
            stmt = con.prepareStatement(consulta.toString());
            
            stmt.setInt(1, equipo.getId());
            stmt.setString(2, equipo.getCompanhia());
            stmt.setString(3, equipo.getNombre());
            stmt.setDate(4, new java.sql.Date(equipo.getFechaAdquisicion().getTime()));
            stmt.setString(5, equipo.getTipo());
            stmt.setInt(6, idGrupo);
            
            if(tipo == Equipo.COMUNICACION){
                stmt.setFloat(7, Float.valueOf(equipo.getPropiedad()));
            }else{
                stmt.setString(7, equipo.getPropiedad());
            }
            
            stmt.executeUpdate();
        }catch (SQLException ex){
            System.out.println(ex.getMessage());
        } finally {
            try {
                if (stmt != null) stmt.close();
            } catch (SQLException ex) {
                System.out.println("Error cerrando recursos: " + ex.getMessage());
            }
        }
    }
    
    public void borrarEquipo(int id, int tipo){
        
        Connection con = this.getConexion();
        PreparedStatement stmt = null;
        
        StringBuilder consulta = new StringBuilder("DELETE FROM equipo");

        switch(tipo){
            case Equipo.COMUNICACION:
                consulta.append( "Comunicacion");
                break;
            case Equipo.MEDICO:
                consulta.append( "Medico");
                break;
            case Equipo.PROTECCION:
                consulta.append( "Proteccion");
                break;
        }
        
        consulta.append( " WHERE id = ? AND idGrupo = ?");
        
        try{
            
            stmt = con.prepareStatement(consulta.toString());            
            stmt.setInt(1, id);
            stmt.setInt(2, idGrupo);
            
            stmt.executeUpdate();
        }catch(SQLException ex){
            System.out.println(ex.getMessage());
        } finally {
            try {
                if (stmt != null) stmt.close();
            } catch (SQLException ex) {
                System.out.println("Error cerrando recursos: " + ex.getMessage());
            }
        }
    }
    
    public void actualizarEquipo(int i, Equipo equipo, int tipo){
        
        Connection con = this.getConexion();
        PreparedStatement stmt = null;
        
        StringBuilder consulta = new StringBuilder("UPDATE equipo");

        switch(tipo){
            case Equipo.COMUNICACION:
                consulta.append( "Comunicacion SET id = ?, companhia = ?, nombre = ?, fechaAdquisicion = ?, tipo = ?, idGrupo = ?, alcance = ? WHERE id = ? and idGrupo = ?");
                break;
            case Equipo.MEDICO:
                consulta.append( "Medico SET id = ?, companhia = ?, nombre = ?, fechaAdquisicion = ?, tipo = ?, idGrupo = ?, categoria = ? WHERE id = ? and idGrupo = ?");
                break;
            case Equipo.PROTECCION:
                consulta.append( "Proteccion SET id = ?, companhia = ?, nombre = ?, fechaAdquisicion = ?, tipo = ?, idGrupo = ?, material = ? WHERE id = ? and idGrupo = ?");
                break;
        }
        
        
        try{
            stmt = con.prepareStatement(consulta.toString());
            
            stmt.setInt(1, equipo.getId());
            stmt.setString(2, equipo.getCompanhia());
            stmt.setString(3, equipo.getNombre());
            stmt.setDate(4, new java.sql.Date(equipo.getFechaAdquisicion().getTime()));
            stmt.setString(5, equipo.getTipo());
            stmt.setInt(6, idGrupo);
            
            if(tipo == Equipo.COMUNICACION){
                stmt.setFloat(7, Float.valueOf(equipo.getPropiedad()));
            }else{
                stmt.setString(7, equipo.getPropiedad());
            }
            
            stmt.setInt(8, equipo.getId());
            stmt.setInt(9, idGrupo);
            
            stmt.executeUpdate();
        }catch (SQLException ex){
            System.out.println(ex.getMessage());
        } finally {
            try {
                if (stmt != null) stmt.close();
            } catch (SQLException ex) {
                System.out.println("Error cerrando recursos: " + ex.getMessage());
            }
        }
    }
    
    public void setGrupo(int id){
        this.idGrupo = id;
    }
}
