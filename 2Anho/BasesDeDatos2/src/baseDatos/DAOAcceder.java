package baseDatos;

import aplicacion.Miembro;
import java.sql.*;
import java.util.*;
import aplicacion.BCrypt;
import aplicacion.Cuartel;
import java.sql.Date;

/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */

/**
 *
 * @author alumnogreibd
 */
public class DAOAcceder extends AbstractDAO{
    
    public DAOAcceder(Connection conexion, aplicacion.FachadaAplicacion fa){
        super.setConexion(conexion);
        super.setFachadaAplicacion(fa);
    }
    
    public void asignarMiembroCuartel(Miembro miembro, Cuartel cuartel){
        Connection con = this.getConexion();
        String consulta = "insert into acceder(hora, fecha, cuartel, idGrupo, dni) values(?,?,?,?,?)";

        try {
            con.setAutoCommit(false); 

            try (PreparedStatement stmt = con.prepareStatement(consulta)) {
                
                
                stmt.setTime(1, java.sql.Time.valueOf(java.time.LocalTime.now()));
                stmt.setDate(2, java.sql.Date.valueOf(java.time.LocalDate.now()));
                stmt.setString(3, cuartel.getNombre());
                stmt.setInt(4, miembro.getIdGrupo());
                stmt.setString(5, miembro.getDni());
                
                stmt.executeUpdate();
                
            } catch (SQLException e) {
                con.rollback();  
            }
        } catch (SQLException e) {
            try {
                con.rollback();  
            } catch (SQLException rollbackEx) {
                System.out.println("Error en rollback: " + rollbackEx.getMessage());
            }
        } finally {
            try {
                con.setAutoCommit(true);  
            } catch (SQLException ex) {
                System.out.println("Error restaurando autocommit: " + ex.getMessage());
            }
        }        
    }
    
    public boolean existeMiembroCuartel(String dni, String cuartel){
        Connection con;
        PreparedStatement stmMiembro = null;
        ResultSet rsMiembro = null;
        boolean existe = false;

        con = this.getConexion();

        try {
            stmMiembro = con.prepareStatement("select count(*) from acceder where dni = ? and cuartel = ?");
            stmMiembro.setString(1, dni);
            stmMiembro.setString(2, cuartel);
            rsMiembro = stmMiembro.executeQuery();

            if (rsMiembro.next()) {
                existe = rsMiembro.getInt(1) > 0; // Si COUNT(*) > 0, el miembro ya existe
            }

        } catch (SQLException e) {
            System.out.println("Error comprobando existencia: " + e.getMessage());
        } finally {
            try {
                if (rsMiembro != null) rsMiembro.close();
                if (stmMiembro != null) stmMiembro.close();
            } catch (SQLException e) {
                System.out.println("Imposible cerrar cursores");
            }
        }

        return existe;
    }
    
     public void borrarAsignacion(String dni, String cuartel){
        Connection con;
        PreparedStatement stmMiembro=null;

        con=super.getConexion();

       try {
        stmMiembro=con.prepareStatement("delete from acceder where dni = ? and cuartel = ?");
        stmMiembro.setString(1, dni);
        stmMiembro.setString(2, cuartel);
        stmMiembro.executeUpdate();
                
            } catch (SQLException e) {
            System.out.println("Error borrando asignacion: " + e.getMessage());
        } finally {
            try {
                if (stmMiembro != null) stmMiembro.close();
            } catch (SQLException e) {
                System.out.println("Imposible cerrar cursores");
            }
        }     
    }
}
