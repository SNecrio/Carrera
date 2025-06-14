/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package baseDatos;
import aplicacion.Armamento;
import java.sql.*;
import java.util.*;
/**
 *
 * @author alumnogreibd
 */
public class DAOArmamento extends AbstractDAO {
    
    private int idGrupo;
    
    public DAOArmamento(Connection conexion, aplicacion.FachadaAplicacion fa){
        super.setConexion(conexion);
        super.setFachadaAplicacion(fa);
        
    }
    
    public ArrayList<Armamento> obtenerArmamento(String numSerie, String modelo, String tipo){
        
        ArrayList<Armamento> resultado = new ArrayList<>();
        Armamento armaActual;
        
        Connection con = this.getConexion();
        PreparedStatement stmCatalogo = null;
        ResultSet rsCatalogo = null;

        StringBuilder consulta = new StringBuilder("SELECT * FROM armamento WHERE 1=1");
        
        if(numSerie != null && !numSerie.isEmpty()){
            consulta.append( " AND numSerie = ?");
        }
        
        if(modelo != null && !modelo.isEmpty()){
            consulta.append(" AND modelo LIKE ?");
        }
        
        if(tipo != null && !tipo.isEmpty()){
            consulta.append(" AND tipo LIKE ?");
        }
        
        consulta.append(" AND idGrupo = ?");
        
        System.out.println("Consulta SQL: " + consulta.toString());

       try {
            stmCatalogo = con.prepareStatement(consulta.toString());

            int paramIndex = 1;

            if (numSerie != null && !numSerie.isEmpty()) {
                stmCatalogo.setInt(paramIndex++, Integer.parseInt(numSerie));
            }
            if (modelo != null && !modelo.isEmpty()) {
                stmCatalogo.setString(paramIndex++, "%" + modelo + "%");
            }
            if (tipo != null && !tipo.isEmpty()) {
                stmCatalogo.setString(paramIndex++, "%" + tipo + "%");
            }
            

            stmCatalogo.setInt(paramIndex++, idGrupo);

            rsCatalogo = stmCatalogo.executeQuery();
            
            
            while (rsCatalogo.next()) {
                System.out.println(rsCatalogo.getString("modelo"));
                armaActual = new Armamento(rsCatalogo.getInt("numSerie"), rsCatalogo.getString("modelo"),
                        rsCatalogo.getString("tipo"), rsCatalogo.getString("estado"), rsCatalogo.getString("ubicacion"),
                        rsCatalogo.getDate("fechaAdquisicion"), rsCatalogo.getInt("idGrupo"));
                
                resultado.add(armaActual);
            }

        } catch (SQLException e) {
            System.out.println("Error al obtener armamento: " + e.getMessage());
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
    
    public void insertarArmamento(Armamento arma){
        
        Connection con = this.getConexion();
        PreparedStatement stmt = null;
        
        try{
            stmt = con.prepareStatement("INSERT INTO armamento(numSerie,tipo,modelo,estado,ubicacion,fechaAdquisicion,idGrupo) VALUES(?,?,?,?,?,?,?)");
            
            stmt.setInt(1, arma.getNumSerie());
            stmt.setString(2, arma.getTipo());
            stmt.setString(3, arma.getModelo());
            stmt.setString(4, arma.getEstado());
            stmt.setString(5, arma.getUbicacion());
            stmt.setDate(6, new java.sql.Date(arma.getFechaAdquisicion().getTime()));
            stmt.setInt(7, idGrupo);
            
            stmt.executeUpdate();
        }catch (SQLException ex){
            System.out.println("No va el insertar el armamento");
            System.out.println(ex.getMessage());
        } finally {
            try {
                if (stmt != null) stmt.close();
            } catch (SQLException ex) {
                System.out.println("Error cerrando recursos: " + ex.getMessage());
            }
        }
    }
    
    public void borrarArmamento(int numSerie){
        
        Connection con = this.getConexion();
        PreparedStatement stmt = null;
        
        try{
            stmt = con.prepareStatement("DELETE FROM armamento WHERE numSerie = ? AND idGrupo = ?");
            
            stmt.setInt(1, numSerie);
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
    
    public void actualizarArmamento(int numSerie, Armamento arma){
        
        Connection con = this.getConexion();
        PreparedStatement stmt = null;
        
        try{
            stmt = con.prepareStatement("UPDATE armamento SET "
                    + "numSerie = ?, tipo = ?, modelo = ?, estado = ?, ubicacion = ?, fechaAdquisicion = ?, idGrupo = ?  WHERE numSerie = ? and idGrupo = ?");
            
            stmt.setInt(1, arma.getNumSerie());
            stmt.setString(2, arma.getTipo());
            stmt.setString(3, arma.getModelo());
            stmt.setString(4, arma.getEstado());
            stmt.setString(5, arma.getUbicacion());
            stmt.setDate(6, new java.sql.Date(arma.getFechaAdquisicion().getTime()));
            stmt.setInt(7, idGrupo);
            stmt.setInt(8, numSerie);
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
        
    public HashMap<String, Integer> obtenerResumen(){
        
        HashMap<String, Integer> mapa = new HashMap<>();
        Connection con = this.getConexion();
        PreparedStatement stmt = null;
        ResultSet rs = null;
        
        try {
            stmt = con.prepareStatement("SELECT tipo, count(tipo) as cantidad FROM armamento GROUP BY tipo");
            
            rs = stmt.executeQuery();
            
            while(rs.next()){
                mapa.put(rs.getString("tipo"), rs.getInt("cantidad"));
            }
        } catch (SQLException ex) {
            System.out.println(ex.getMessage());
        }
        
        return mapa;
    }
}
