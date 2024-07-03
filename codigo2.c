using System;
using System.Diagnostics;
using System.Reflection.Metadata;
using Npgsql;
using Npgsql.Internal;
using iText.Kernel.Pdf;
using iText.Layout;
using Document = iText.Layout.Document;
using iText.Layout.Element;
using System.Text;
using iText.Kernel.Colors;
using iText.Layout.Properties;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Threading;
using System.Linq;

namespace CRUDUsuariosRoles
{
    class Program
    {
        static string connectionString = "Host=localhost;Username=postgres;Password=19982012;Database=restaurante";
        static void Main(string[] args)
        {
            while (true)
            {
                Console.WriteLine("");
                Console.WriteLine("Seleccione una opción:");
                Console.WriteLine("1. Crear usuario");
                Console.WriteLine("2. Modificar usuario");
                Console.WriteLine("3. Eliminar usuario");
                Console.WriteLine("4. Crear rol");
                Console.WriteLine("5. Asignar rol a usuario");
                Console.WriteLine("6. Consultar roles");
                Console.WriteLine("7. Consultar usuarios");
                Console.WriteLine("8. Realizar respaldo");
                Console.WriteLine("9. Restaurar Db");
                Console.WriteLine("10.Listar entidades");
                Console.WriteLine("11.Listar atributos por entidad");
                Console.WriteLine("12.Agregar Entidad ConAtributos");
                Console.WriteLine("13.Generar informe pdf");
                Console.WriteLine("14.Procedmientos almacenados");
                Console.WriteLine("15. Salir");
                Console.WriteLine("16. Generar tabla de auditoría personalizada"); // Nueva opción
                Console.WriteLine("17. Generar informe de auditoria");// Nueva opción
                Console.WriteLine("18. listar atributos con sus identidades y generar disparadores"); // Nueva opción
     
                Console.WriteLine("19. Gestionar logs de eventos y auditoría");// Nueva opción

                Console.WriteLine("");
                Console.Write("Ingrese la opción seleccionada: ");

                int opcion;
                if (!int.TryParse(Console.ReadLine(), out opcion))
                {
                    Console.WriteLine("Opción no válida. Por favor, ingrese un número.");
                    continue;
                }

                switch (opcion)
                {
                    case 1:
                        CrearUsuario();
                        break;
                    case 2:
                        ModificarUsuario();
                        break;
                    case 3:
                        EliminarUsuario();
                        break;
                    case 4:
                        CrearRol();
                        break;
                    case 5:
                        AsignarRolUsuario();
                        break;
                    case 6:
                        ConsultarRoles();
                        break;
                    case 7:
                        ConsultarUsuarios();
                        break;
                    case 8:
                        RealizarRespaldo();
                        break;
                    case 9:
                        RestaurarBaseDeDatos();
                        break;
                    case 10:
                        ListarEntidadesBaseDatos();
                        break;
                    case 11:
                        ListarAtributosEntidad();
                        break;
                    case 12:
                        AgregarEntidadConAtributos();
                        break;
                    case 13:
                        GenerarInformePDF();
                        break;
                    case 14:
                        GenerarProcedimientosAlmacenados();
                        break;
                    case 15:
                        return;
                    case 16:
                        GenerarTablaAuditoriaPersonalizada(); // Nuevo caso
                        break;
                    case 17:
                        GenerarInformeDeAuditoria();// Nuevo caso
                        break;
                    case 18:
                        GenerarDisparadoresYTablaAuditoria(); // Nuevo caso
                        break;
            
                    case 19:
                        GestionarLogsEventosAuditoria();// Nuevo caso
                        break;
                    default:
                        Console.WriteLine("Opción no válida.");
                        break;
                }
            }
        }
       static void GestionarLogsEventosAuditoria()
    {
        while (true)
        {
            Console.WriteLine("\nGestión de Logs de Eventos y Auditoría");
            Console.WriteLine("1. Acceder a logs de eventos y auditoría");
            Console.WriteLine("2. Agregar filtros");
            Console.WriteLine("3. Generar informe PDF con información filtrada");
            Console.WriteLine("4. Volver al menú principal");
            Console.Write("Seleccione una opción: ");

            if (int.TryParse(Console.ReadLine(), out int opcion))
            {
                switch (opcion)
                {
                    case 1:
                        AccederLogs();
                        break;
                    case 2:
                        AgregarFiltros();
                        break;
                    case 3:
                        GenerarInformePDFFiltrado();
                        break;
                    case 4:
                        return;
                    default:
                        Console.WriteLine("Opción no válida.");
                        break;
                }
            }
            else
            {
                Console.WriteLine("Por favor, ingrese un número válido.");
            }
        }
    }

    static List<string> filtros = new List<string>();

        static void AccederLogs()
        {
            try
            {
                using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
                {
                    connection.Open();
                    string query = "SELECT * FROM auditoria";

                    if (filtros.Count > 0)
                    {
                        query += " WHERE " + string.Join(" AND ", filtros);
                    }

                    using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                    using (NpgsqlDataReader reader = command.ExecuteReader())
                    {
                        bool hasRows = false;
                        while (reader.Read())
                        {
                            hasRows = true;
                            Console.WriteLine($"ID: {reader["id"]}, Tabla: {reader["nombre_tabla"]}, Operación: {reader["operacion"]}, Usuario: {reader["usuario_actual"]}, Fecha: {reader["fecha_hora"]}, Detalle Anterior: {reader["detalle_anterior"]}, Detalle Nuevo: {reader["detalle_nuevo"]}");
                        }

                        if (!hasRows)
                        {
                            Console.WriteLine("No se encontraron registros con los filtros aplicados.");
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al acceder a los logs: {ex.Message}");
            }
        }


        static void AgregarFiltros()
    {
        Console.WriteLine("Ingrese el filtro en formato SQL (ejemplo: operacion = 'INSERT'):");
        string filtro = Console.ReadLine();

        if (ValidarFiltro(filtro))
        {
            filtros.Add(filtro);
            Console.WriteLine("Filtro agregado correctamente.");
        }
        else
        {
            Console.WriteLine("Filtro no válido. Por favor, ingrese un filtro en formato SQL correcto.");
        }
    }

    static bool ValidarFiltro(string filtro)
    {
        string[] validColumns = { "id", "nombre_tabla", "operacion", "usuario_actual", "fecha_hora", "detalle_nuevo" };
        string[] filtroParts = filtro.Split(new[] { '=' }, StringSplitOptions.RemoveEmptyEntries);

        if (filtroParts.Length == 2 && validColumns.Contains(filtroParts[0].Trim()))
        {
            return true;
        }

        return false;
    }

    static void GenerarInformePDFFiltrado()
    {
        try
        {
            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                connection.Open();
                string query = "SELECT * FROM auditoria";

                if (filtros.Count > 0)
                {
                    query += " WHERE " + string.Join(" AND ", filtros);
                }

                List<Dictionary<string, string>> datos = new List<Dictionary<string, string>>();

                using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                using (NpgsqlDataReader reader = command.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        datos.Add(new Dictionary<string, string>
                        {
                            {"ID", reader["id"].ToString()},
                            {"Tabla", reader["nombre_tabla"].ToString()},
                            {"Operación", reader["operacion"].ToString()},
                            {"Usuario", reader["usuario_actual"].ToString()},
                            {"Fecha", reader["fecha_hora"].ToString()},
                            {"Detalle", reader["detalle_nuevo"].ToString()}
                        });
                    }
                }

                string rutaPDF = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), "InformeLogsFiltrados.pdf");

                using (PdfWriter writer = new PdfWriter(rutaPDF))
                using (PdfDocument pdf = new PdfDocument(writer))
                using (Document document = new Document(pdf))
                {
                    document.Add(new Paragraph("Informe de Logs de Eventos y Auditoría (Filtrado)"));
                    document.Add(new Paragraph($"Fecha de generación: {DateTime.Now}"));
                    document.Add(new Paragraph($"Filtros aplicados: {string.Join(", ", filtros)}"));
                    document.Add(new Paragraph("\n"));

                    Table table = new Table(6);
                    table.AddHeaderCell("ID");
                    table.AddHeaderCell("Tabla");
                    table.AddHeaderCell("Operación");
                    table.AddHeaderCell("Usuario");
                    table.AddHeaderCell("Fecha");
                    table.AddHeaderCell("Detalle");

                    foreach (var dato in datos)
                    {
                        table.AddCell(dato["ID"]);
                        table.AddCell(dato["Tabla"]);
                        table.AddCell(dato["Operación"]);
                        table.AddCell(dato["Usuario"]);
                        table.AddCell(dato["Fecha"]);
                        table.AddCell(dato["Detalle"]);
                    }

                    document.Add(table);
                }

                Console.WriteLine($"Informe PDF generado: {rutaPDF}");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error al generar el informe PDF: {ex.Message}");
        }
    }






















    





















    static void GenerarTablaAuditoriaPersonalizada()
        {
            Console.Write("Ingrese el nombre de la nueva tabla de auditoría: ");
            string nombreTabla = Console.ReadLine();

            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                try
                {
                    connection.Open();
                    string query = $@"
                        CREATE TABLE {nombreTabla} (
                            id SERIAL PRIMARY KEY,
                            operacion VARCHAR(10),
                            nombre_tabla VARCHAR(255) NOT NULL,
                            fecha_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                            usuario_actual VARCHAR(255) NOT NULL,
                            detalle_accion TEXT NOT NULL
                            detalle_nuevo TEXT NOT NULL
                        );";

                    using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                    {
                        command.ExecuteNonQuery();
                        Console.WriteLine($"Tabla '{nombreTabla}' creada exitosamente.");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error al crear la tabla: {ex.Message}");
                }
            }
        }









        static void GenerarDisparadoresYTablaAuditoria()
        {
            try
            {
                using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
                {
                    connection.Open();

                    // Crear una única tabla de auditoría
                    string crearTablaAuditoria = @"
            CREATE TABLE IF NOT EXISTS auditoria (
                id SERIAL PRIMARY KEY,
                nombre_tabla VARCHAR(255),
                operacion VARCHAR(10),
                usuario_actual VARCHAR(255),
                fecha_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                detalle_anterior TEXT,
                detalle_nuevo TEXT
            );";

                    using (var cmd = new NpgsqlCommand(crearTablaAuditoria, connection))
                    {
                        cmd.ExecuteNonQuery();
                    }

                    Console.WriteLine("Tabla de auditoría creada o ya existente.");

                    // Obtener la lista de tablas en la base de datos
                    List<string> tablas = ListarEntidadesConAtributos(connection);

                    foreach (string tabla in tablas)
                    {
                        if (tabla != "auditoria") // Evitar crear triggers para la tabla de auditoría
                        {
                            // Generar los disparadores para cada tabla
                            string funcionAuditoria = GenerarFuncionAuditoria(tabla);
                            string disparadorInsert = GenerarDisparadorInsert(tabla);
                            string disparadorUpdate = GenerarDisparadorUpdate(tabla);
                            string disparadorDelete = GenerarDisparadorDelete(tabla);

                            // Crear los disparadores en la base de datos
                            using (var cmd = new NpgsqlCommand())
                            {
                                cmd.Connection = connection;

                                // Crear función de auditoría
                                cmd.CommandText = funcionAuditoria;
                                cmd.ExecuteNonQuery();

                                // Crear disparador INSERT
                                cmd.CommandText = disparadorInsert;
                                cmd.ExecuteNonQuery();

                                // Crear disparador UPDATE
                                cmd.CommandText = disparadorUpdate;
                                cmd.ExecuteNonQuery();

                                // Crear disparador DELETE
                                cmd.CommandText = disparadorDelete;
                                cmd.ExecuteNonQuery();
                            }

                            Console.WriteLine($"Disparadores generados y creados para la tabla '{tabla}' en la base de datos.");
                        }
                    }

                    // Preguntar al usuario si desea descargar los disparadores
                    Console.Write("¿Desea descargar los disparadores generados? (s/n): ");
                    string respuesta = Console.ReadLine().ToLower();

                    if (respuesta == "s" || respuesta == "si")
                    {
                        string downloadsFolder = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile) + @"\Downloads";

                        foreach (string tabla in tablas)
                        {
                            if (tabla != "auditoria")
                            {
                                string funcionAuditoria = GenerarFuncionAuditoria(tabla);
                                string disparadorInsert = GenerarDisparadorInsert(tabla);
                                string disparadorUpdate = GenerarDisparadorUpdate(tabla);
                                string disparadorDelete = GenerarDisparadorDelete(tabla);

                                // Guardar los disparadores en archivos separados
                                File.WriteAllText(Path.Combine(downloadsFolder, $"FuncionAuditoria_{tabla}.sql"), funcionAuditoria);
                                File.WriteAllText(Path.Combine(downloadsFolder, $"DisparadorInsert_{tabla}.sql"), disparadorInsert);
                                File.WriteAllText(Path.Combine(downloadsFolder, $"DisparadorUpdate_{tabla}.sql"), disparadorUpdate);
                                File.WriteAllText(Path.Combine(downloadsFolder, $"DisparadorDelete_{tabla}.sql"), disparadorDelete);
                            }
                        }

                        Console.WriteLine($"Disparadores guardados en la carpeta de descargas.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al generar los disparadores: " + ex.Message);
            }
        }
        static string GenerarFuncionAuditoria(string tabla)
        {
            return $@"
CREATE OR REPLACE FUNCTION fn_auditar_{tabla}()
RETURNS TRIGGER AS $$
BEGIN
    IF (TG_OP = 'DELETE') THEN
        INSERT INTO auditoria (nombre_tabla, operacion, usuario_actual, detalle_anterior)
        VALUES ('{tabla}', 'DELETE', current_user, row_to_json(OLD)::text);
    ELSIF (TG_OP = 'UPDATE') THEN
        INSERT INTO auditoria (nombre_tabla, operacion, usuario_actual, detalle_anterior, detalle_nuevo)
        VALUES ('{tabla}', 'UPDATE', current_user, row_to_json(OLD)::text, row_to_json(NEW)::text);
    ELSIF (TG_OP = 'INSERT') THEN
        INSERT INTO auditoria (nombre_tabla, operacion, usuario_actual, detalle_nuevo)
        VALUES ('{tabla}', 'INSERT', current_user, row_to_json(NEW)::text);
    END IF;
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;";
        }

        static string GenerarDisparadorInsert(string tabla)
        {
            return $@"
DROP TRIGGER IF EXISTS trg_auditar_{tabla}_insert ON {tabla};
CREATE TRIGGER trg_auditar_{tabla}_insert
AFTER INSERT ON {tabla}
FOR EACH ROW EXECUTE FUNCTION fn_auditar_{tabla}();";
        }

        static string GenerarDisparadorUpdate(string tabla)
        {
            return $@"
DROP TRIGGER IF EXISTS trg_auditar_{tabla}_update ON {tabla};
CREATE TRIGGER trg_auditar_{tabla}_update
AFTER UPDATE ON {tabla}
FOR EACH ROW EXECUTE FUNCTION fn_auditar_{tabla}();";
        }

        static string GenerarDisparadorDelete(string tabla)
        {
            return $@"
DROP TRIGGER IF EXISTS trg_auditar_{tabla}_delete ON {tabla};
CREATE TRIGGER trg_auditar_{tabla}_delete
AFTER DELETE ON {tabla}
FOR EACH ROW EXECUTE FUNCTION fn_auditar_{tabla}();";
        }










        static void GenerarInformeDeAuditoria()
        {
            try
            {
                using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
                {
                    connection.Open();

                    // Obtener la lista de tablas
                    List<string> tablas = ListarEntidadesConAtributos(connection);

                    Console.Write("Seleccione el número de la tabla para generar el informe de auditoría: ");
                    if (int.TryParse(Console.ReadLine(), out int seleccionTabla) && seleccionTabla > 0 && seleccionTabla <= tablas.Count)
                    {
                        string tablaSeleccionada = tablas[seleccionTabla - 1];

                        // Obtener los atributos de la tabla seleccionada
                        List<string> atributos = ObtenerAtributosTabla(connection, tablaSeleccionada);

                        // Solicitar al usuario que seleccione los atributos para el informe
                        List<string> atributosSeleccionados = SolicitarAtributos(atributos);

                        // Crear una lista para almacenar los resultados de los hilos
                        List<List<Dictionary<string, object>>> resultadosHilos = new List<List<Dictionary<string, object>>>();

                        // Dividir la consulta en múltiples hilos
                        int numeroHilos = 4; // Puedes ajustar este número según tus necesidades
                        int registrosPorHilo = ObtenerTotalRegistros(connection, tablaSeleccionada) / numeroHilos;

                        List<Thread> hilos = new List<Thread>();

                        for (int i = 0; i < numeroHilos; i++)
                        {
                            int offset = i * registrosPorHilo;
                            Thread hilo = new Thread(() =>
                            {
                                List<Dictionary<string, object>> resultadoHilo = EjecutarConsultaParcial(tablaSeleccionada, atributosSeleccionados, offset, registrosPorHilo);
                                lock (resultadosHilos)
                                {
                                    resultadosHilos.Add(resultadoHilo);
                                }
                            });
                            hilos.Add(hilo);
                            hilo.Start();
                        }

                        // Esperar a que todos los hilos terminen
                        foreach (Thread hilo in hilos)
                        {
                            hilo.Join();
                        }

                        // Combinar los resultados de todos los hilos
                        List<Dictionary<string, object>> resultadosFinal = resultadosHilos.SelectMany(r => r).ToList();

                        // Generar el informe
                        GenerarInformePDF(tablaSeleccionada, atributosSeleccionados, resultadosFinal);

                        Console.WriteLine("Informe de auditoría generado con éxito.");
                    }
                    else
                    {
                        Console.WriteLine("Selección no válida.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al generar el informe de auditoría: " + ex.Message);
            }
        }

        static List<string> ObtenerAtributosTabla(NpgsqlConnection connection, string tabla)
        {
            List<string> atributos = new List<string>();
            string query = $"SELECT column_name FROM information_schema.columns WHERE table_name = '{tabla}'";

            using (NpgsqlCommand cmd = new NpgsqlCommand(query, connection))
            using (NpgsqlDataReader reader = cmd.ExecuteReader())
            {
                while (reader.Read())
                {
                    atributos.Add(reader.GetString(0));
                }
            }

            return atributos;
        }

        static List<string> SolicitarAtributos(List<string> atributos)
        {
            Console.WriteLine("Atributos disponibles:");
            for (int i = 0; i < atributos.Count; i++)
            {
                Console.WriteLine($"{i + 1}. {atributos[i]}");
            }

            Console.Write("Ingrese los números de los atributos que desea incluir en el informe (separados por coma): ");
            string[] seleccion = Console.ReadLine().Split(',');

            return seleccion.Select(s => atributos[int.Parse(s.Trim()) - 1]).ToList();
        }

        static int ObtenerTotalRegistros(NpgsqlConnection connection, string tabla)
        {
            string query = $"SELECT COUNT(*) FROM {tabla}";
            using (NpgsqlCommand cmd = new NpgsqlCommand(query, connection))
            {
                return Convert.ToInt32(cmd.ExecuteScalar());
            }
        }

        static List<Dictionary<string, object>> EjecutarConsultaParcial(string tabla, List<string> atributos, int offset, int limit)
        {
            List<Dictionary<string, object>> resultado = new List<Dictionary<string, object>>();

            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                connection.Open();
                string query = $"SELECT {string.Join(", ", atributos)} FROM {tabla} OFFSET {offset} LIMIT {limit}";

                using (NpgsqlCommand cmd = new NpgsqlCommand(query, connection))
                using (NpgsqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Dictionary<string, object> fila = new Dictionary<string, object>();
                        foreach (string atributo in atributos)
                        {
                            fila[atributo] = reader[atributo];
                        }
                        resultado.Add(fila);
                    }
                }
            }

            return resultado;
        }

        static void GenerarInformePDF(string tabla, List<string> atributos, List<Dictionary<string, object>> datos)
        {
            string rutaPDF = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), $"InformeAuditoria_{tabla}.pdf");

            using (PdfWriter writer = new PdfWriter(rutaPDF))
            using (PdfDocument pdf = new PdfDocument(writer))
            using (Document document = new Document(pdf))
            {
                document.Add(new Paragraph($"Informe de Auditoría - Tabla: {tabla}"));
                document.Add(new Paragraph($"Fecha: {DateTime.Now}"));
                document.Add(new Paragraph("\n"));

                Table table = new Table(atributos.Count);
                foreach (string atributo in atributos)
                {
                    table.AddHeaderCell(atributo);
                }

                foreach (var fila in datos)
                {
                    foreach (string atributo in atributos)
                    {
                        table.AddCell(fila[atributo].ToString());
                    }
                }

                document.Add(table);
            }

            Console.WriteLine($"Informe PDF generado: {rutaPDF}");
        }

















        static void ListarAtributos(NpgsqlConnection connection, string tableName)
        {
            string query = $"SELECT column_name, data_type FROM information_schema.columns WHERE table_name = '{tableName}'";

            using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
            using (NpgsqlDataReader reader = command.ExecuteReader())
            {
                while (reader.Read())
                {
                    string columnName = reader.GetString(0);
                    string dataType = reader.GetString(1);
                    Console.WriteLine($"   - {columnName} ({dataType})");
                }
                reader.Close();  // Explícitamente cerrar el DataReader
            }
        }

        static void CrearUsuario()
        {
            Console.Write("Ingrese el nombre del nuevo usuario: ");
            string nombreUsuario = Console.ReadLine();

            Console.Write("Ingrese la contraseña del nuevo usuario: ");
            string contraseñaUsuario = Console.ReadLine();

            Console.Write("Ingrese el nombre de la base de datos para crear el usuario: ");
            string nombreBaseDeDatos = Console.ReadLine();

            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                connection.Open();

                // Verificar si la base de datos existe
                string verificarBaseDeDatosQuery = $"SELECT datname FROM pg_database WHERE datname = @nombreBaseDeDatos";
                using (NpgsqlCommand verificarBaseDeDatosCmd = new NpgsqlCommand(verificarBaseDeDatosQuery, connection))
                {
                    verificarBaseDeDatosCmd.Parameters.AddWithValue("@nombreBaseDeDatos", nombreBaseDeDatos);
                    object resultado = verificarBaseDeDatosCmd.ExecuteScalar();
                    if (resultado == null)
                    {
                        Console.WriteLine($"La base de datos '{nombreBaseDeDatos}' no existe en el servidor.");
                        return;
                    }
                }

                // Crear el usuario
                string createUsuarioQuery = $"CREATE USER {nombreUsuario} WITH PASSWORD '{contraseñaUsuario}'";
                using (NpgsqlCommand createUsuarioCommand = new NpgsqlCommand(createUsuarioQuery, connection))
                {
                    try
                    {
                        createUsuarioCommand.ExecuteNonQuery();
                        Console.WriteLine($"Usuario '{nombreUsuario}' creado correctamente en la base de datos '{nombreBaseDeDatos}'.");
                        Console.WriteLine(" ");
                    }
                    catch (NpgsqlException ex)
                    {
                        Console.WriteLine($"Error al crear el usuario: {ex.Message}");
                        return;
                    }
                }
            }
        }

        static void ModificarUsuario()
        {
            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                connection.Open();

                // Obtener la lista de usuarios disponibles en la base de datos
                Dictionary<int, string> usuarios = new Dictionary<int, string>();
                string obtenerUsuariosQuery = "SELECT usesysid, usename FROM pg_user";
                using (NpgsqlCommand obtenerUsuariosCmd = new NpgsqlCommand(obtenerUsuariosQuery, connection))
                {
                    using (NpgsqlDataReader reader = obtenerUsuariosCmd.ExecuteReader())
                    {
                        int index = 1;
                        while (reader.Read())

                        {
                            object usuarioIdObject = reader.GetValue(0); // Leer el valor como un objeto
                            string nombreUser = reader.GetString(1); // Obtener el nombre de usuario
                            usuarios.Add(index, nombreUser);
                            Console.WriteLine($"{index}. {nombreUser}");
                            index++;
                        }
                    }
                }

                // Solicitar al usuario que seleccione el número del usuario a modificar
                Console.Write("\nIngrese el número del usuario a modificar: ");
                if (!int.TryParse(Console.ReadLine(), out int seleccion))
                {
                    Console.WriteLine("Entrada no válida. Debe ingresar un número.");
                    return;
                }

                // Verificar si el número de usuario seleccionado existe
                if (!usuarios.ContainsKey(seleccion))
                {
                    Console.WriteLine("Número de usuario no válido.");
                    return;
                }

                string nombreUsuario = usuarios[seleccion];

                // Solicitar al usuario que ingrese el nuevo nombre para el usuario seleccionado
                Console.Write("Ingrese el nuevo nombre para el usuario: ");
                string nuevoNombreUsuario = Console.ReadLine();

                // Modificar el nombre del usuario
                string query = $"ALTER USER \"{nombreUsuario}\" RENAME TO \"{nuevoNombreUsuario}\"";
                using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                {
                    try
                    {
                        command.ExecuteNonQuery();
                        Console.WriteLine($"Usuario '{nombreUsuario}' modificado correctamente. Nuevo nombre: '{nuevoNombreUsuario}'");
                    }
                    catch (NpgsqlException ex)
                    {
                        Console.WriteLine($"Error al modificar el usuario: {ex.Message}");
                    }
                }
            }
        }

        static void EliminarUsuario()
        {
            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                connection.Open();

                // Obtener la lista de usuarios disponibles en la base de datos
                Dictionary<int, string> usuarios = new Dictionary<int, string>();
                string obtenerUsuariosQuery = "SELECT usesysid, usename FROM pg_user";
                using (NpgsqlCommand obtenerUsuariosCmd = new NpgsqlCommand(obtenerUsuariosQuery, connection))
                {
                    using (NpgsqlDataReader reader = obtenerUsuariosCmd.ExecuteReader())
                    {
                        int index = 1;
                        while (reader.Read())
                        {
                            //object usuarioIdObject = reader.GetValue(0); // Leer el valor como un objeto

                            string nombreUser = reader.GetString(1); // Obtener el nombre de usuario
                            usuarios.Add(index, nombreUser);
                            Console.WriteLine($"{index}. {nombreUser}");
                            index++;
                        }
                    }
                }

                // Solicitar al usuario que seleccione el número del usuario a eliminar
                Console.Write("\nIngrese el número del usuario a eliminar: ");
                if (!int.TryParse(Console.ReadLine(), out int seleccion))
                {
                    Console.WriteLine("Entrada no válida. Debe ingresar un número.");
                    return;
                }

                // Verificar si el número de usuario seleccionado existe
                if (!usuarios.ContainsKey(seleccion))
                {
                    Console.WriteLine("Número de usuario no válido.");
                    return;
                }

                string nombreUsuario = usuarios[seleccion];

                // Eliminar el usuario
                string eliminarUsuarioQuery = $"DROP USER \"{nombreUsuario}\"";
                using (NpgsqlCommand eliminarUsuarioCmd = new NpgsqlCommand(eliminarUsuarioQuery, connection))
                {
                    try
                    {
                        eliminarUsuarioCmd.ExecuteNonQuery();
                        Console.WriteLine($"Usuario '{nombreUsuario}' eliminado correctamente.");
                    }
                    catch (NpgsqlException ex)
                    {
                        Console.WriteLine($"Error al eliminar el usuario: {ex.Message}");
                    }
                }
            }
        }
        static void CrearRol()
        {
            Console.Write("Ingrese el nombre del nuevo rol: ");
            string nombreRol = Console.ReadLine();

            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                connection.Open();

                // Verificar si el rol ya existe
                string verificarRolQuery = $"SELECT rolname FROM pg_roles WHERE rolname = @nombreRol";
                using (NpgsqlCommand verificarRolCmd = new NpgsqlCommand(verificarRolQuery, connection))
                {
                    verificarRolCmd.Parameters.AddWithValue("@nombreRol", nombreRol);
                    object resultado = verificarRolCmd.ExecuteScalar();
                    if (resultado != null)
                    {
                        Console.WriteLine($"El rol '{nombreRol}' ya existe.");
                        return;
                    }
                }

                string query = $"CREATE ROLE \"{nombreRol}\"";
                NpgsqlCommand command = new NpgsqlCommand(query, connection);

                try
                {
                    command.ExecuteNonQuery();
                    Console.WriteLine($"Rol '{nombreRol}' creado correctamente.");
                    Console.WriteLine(" ");
                }
                catch (NpgsqlException ex)
                {
                    Console.WriteLine($"Error al crear el rol: {ex.Message}");
                }
            }
        }
        static void AsignarRolUsuario()
        {
            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                connection.Open();

                // Obtener la lista de usuarios disponibles en la base de datos
                Dictionary<int, string> usuarios = new Dictionary<int, string>();
                string obtenerUsuariosQuery = "SELECT usename FROM pg_user";
                using (NpgsqlCommand obtenerUsuariosCmd = new NpgsqlCommand(obtenerUsuariosQuery, connection))
                {
                    using (NpgsqlDataReader reader = obtenerUsuariosCmd.ExecuteReader())
                    {
                        int index = 1;
                        Console.WriteLine("Usuarios disponibles:");
                        while (reader.Read())
                        {
                            string nombreUser = reader.GetString(0);
                            usuarios.Add(index, nombreUser);
                            Console.WriteLine($"{index}. {nombreUser}");
                            index++;
                        }
                    }
                }

                // Solicitar al usuario que seleccione el número del usuario
                Console.Write("\nIngrese el número del usuario: ");
                if (!int.TryParse(Console.ReadLine(), out int seleccionUsuario))
                {
                    Console.WriteLine("Entrada no válida. Debe ingresar un número.");
                    return;
                }

                // Verificar si el número de usuario seleccionado existe
                if (!usuarios.ContainsKey(seleccionUsuario))
                {
                    Console.WriteLine("Número de usuario no válido.");
                    return;
                }

                string nombreUsuario = usuarios[seleccionUsuario];

                // Obtener la lista de roles disponibles en la base de datos
                Dictionary<int, string> roles = new Dictionary<int, string>();
                string obtenerRolesQuery = "SELECT rolname FROM pg_roles WHERE rolname != 'postgres'";
                using (NpgsqlCommand obtenerRolesCmd = new NpgsqlCommand(obtenerRolesQuery, connection))
                {
                    using (NpgsqlDataReader reader = obtenerRolesCmd.ExecuteReader())
                    {
                        int index = 1;
                        Console.WriteLine("\nRoles disponibles:");
                        while (reader.Read())
                        {
                            string nameRol = reader.GetString(0);
                            roles.Add(index, nameRol);
                            Console.WriteLine($"{index}. {nameRol}");
                            index++;
                        }
                    }
                }

                // Solicitar al usuario que seleccione el número del rol
                Console.Write("\nIngrese el número del rol: ");
                if (!int.TryParse(Console.ReadLine(), out int seleccionRol))
                {
                    Console.WriteLine("Entrada no válida. Debe ingresar un número.");
                    return;
                }

                // Verificar si el número de rol seleccionado existe
                if (!roles.ContainsKey(seleccionRol))
                {
                    Console.WriteLine("Número de rol no válido.");
                    return;
                }

                string nombreRol = roles[seleccionRol];

                // Asignar el rol al usuario
                string query = $"GRANT \"{nombreRol}\" TO \"{nombreUsuario}\"";
                using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                {
                    try
                    {
                        command.ExecuteNonQuery();
                        Console.WriteLine($"Rol '{nombreRol}' asignado al usuario '{nombreUsuario}' correctamente.");
                    }
                    catch (NpgsqlException ex)
                    {
                        Console.WriteLine($"Error al asignar el rol al usuario: {ex.Message}");
                    }
                }
            }
        }

        static void ConsultarRoles()
        {
            Console.WriteLine("Roles en la base de datos:");
            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                connection.Open();

                string query = "SELECT rolname FROM pg_roles WHERE rolname != 'postgres'";
                using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                {
                    using (NpgsqlDataReader reader = command.ExecuteReader())
                    {
                        Console.WriteLine("");
                        while (reader.Read())
                        {
                            Console.WriteLine(reader.GetString(0));
                        }
                    }
                }
            }
        }

        static void ConsultarUsuarios()
        {
            Console.WriteLine("Usuarios en la base de datos:");
            using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
            {
                connection.Open();

                string query = "SELECT usename FROM pg_catalog.pg_user";
                using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                {
                    using (NpgsqlDataReader reader = command.ExecuteReader())
                    {
                        Console.WriteLine("");
                        while (reader.Read())
                        {
                            Console.WriteLine(reader.GetString(0));
                        }
                    }
                }
            }
        }
        static void RealizarRespaldo()
        {
            try
            {
                Console.Write("Ingrese el nombre de la base de datos a respaldar: ");
                string databaseName = Console.ReadLine();

                Console.Write("Ingrese el nombre del archivo de respaldo (.sql): ");
                string fileName = Console.ReadLine();

                // Ruta de la carpeta de respaldo (escritorio)
                string desktopFolder = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
                string backupFilePath = Path.Combine(desktopFolder, fileName);

                if (File.Exists(backupFilePath))
                {
                    Console.WriteLine("El archivo de respaldo ya existe en la ubicación especificada.");
                    return;
                }

                // Comando pg_dump para realizar el respaldo
                string pgDumpPath = @"C:\Program Files\PostgreSQL\16\bin\pg_dump.exe"; // Ruta al ejecutable pg_dump
                string pgDumpCommand = $"-U postgres -h localhost -f \"{backupFilePath}\" {databaseName}";

                // Ejecutar el comando pg_dump utilizando ProcessStartInfo
                using (Process pgDumpProcess = new Process())
                {
                    pgDumpProcess.StartInfo.FileName = pgDumpPath;
                    pgDumpProcess.StartInfo.Arguments = pgDumpCommand;
                    pgDumpProcess.StartInfo.UseShellExecute = false;
                    pgDumpProcess.StartInfo.RedirectStandardOutput = true;
                    pgDumpProcess.StartInfo.RedirectStandardError = true;

                    pgDumpProcess.Start();

                    // Capturar la salida estándar y la salida de error
                    string output = pgDumpProcess.StandardOutput.ReadToEnd();
                    string error = pgDumpProcess.StandardError.ReadToEnd();

                    pgDumpProcess.WaitForExit();

                    if (pgDumpProcess.ExitCode == 0)
                    {
                        Console.WriteLine($"Respaldo de la base de datos '{databaseName}' creado correctamente en '{backupFilePath}'.");
                    }
                    else
                    {
                        Console.WriteLine($"Error al realizar el respaldo: {error}");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al realizar el respaldo: {ex.Message}");
            }
        }
        static void RestaurarBaseDeDatos()
        {
            try
            {
                // Ruta donde se encuentran los archivos de respaldo
                string backupPath = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);

                // Obtener todos los archivos de respaldo disponibles
                string[] backupFiles = Directory.GetFiles(backupPath, "*.sql");

                if (backupFiles.Length > 0)
                {
                    Console.WriteLine("Seleccione el archivo de respaldo a restaurar:");
                    for (int i = 0; i < backupFiles.Length; i++)
                    {
                        Console.WriteLine($"{i + 1}. {Path.GetFileName(backupFiles[i])}");
                    }

                    Console.Write("Ingrese el número correspondiente: ");
                    if (int.TryParse(Console.ReadLine(), out int selectedIndex) && selectedIndex >= 1 && selectedIndex <= backupFiles.Length)
                    {
                        string backupFilePath = backupFiles[selectedIndex - 1];
                        string backupFileName = Path.GetFileName(backupFilePath);

                        // Obtener el nombre de la base de datos del nombre del archivo
                        //string dbName = "SistemaVentas";
                        string dbName = "restaurante";
                        // Comando para restaurar la base de datos utilizando psql
                        string psqlPath = @"C:\Program Files\PostgreSQL\16\bin\psql.exe";
                        string psqlCommand = $"-U postgres -d {dbName} -f \"{backupFilePath}\"";

                        // Ejecutar el comando psql utilizando ProcessStartInfo
                        using (Process psqlProcess = new Process())
                        {
                            psqlProcess.StartInfo.FileName = psqlPath;
                            psqlProcess.StartInfo.Arguments = psqlCommand;
                            psqlProcess.StartInfo.UseShellExecute = false;
                            psqlProcess.StartInfo.RedirectStandardOutput = true;
                            psqlProcess.StartInfo.RedirectStandardError = true;

                            psqlProcess.Start();

                            // Capturar la salida estándar y la salida de error
                            string output = psqlProcess.StandardOutput.ReadToEnd();
                            string error = psqlProcess.StandardError.ReadToEnd();

                            psqlProcess.WaitForExit();

                            if (psqlProcess.ExitCode == 0)
                            {
                                Console.WriteLine($"Se restauró la base de datos '{dbName}' desde el archivo '{backupFileName}'.");
                            }
                            else
                            {
                                Console.WriteLine($"Error al restaurar la base de datos: {error}");
                            }
                        }
                    }
                    else
                    {
                        Console.WriteLine("Selección inválida.");
                    }
                }
                else
                {
                    Console.WriteLine("No hay archivos de respaldo disponibles.");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error al restaurar la base de datos: {ex.Message}");
            }
        }
        static void ListarEntidadesBaseDatos()
        {
            try
            {

                using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
                {
                    connection.Open();

                    // Consulta SQL para obtener la lista de entidades de la base de datos
                    string query = "SELECT tablename FROM pg_catalog.pg_tables WHERE schemaname = 'public'";

                    using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                    {
                        using (NpgsqlDataReader reader = command.ExecuteReader())
                        {
                            Console.WriteLine("Lista de entidades en la base de datos:");

                            // Iterar sobre los resultados y mostrar el nombre de cada entidad
                            while (reader.Read())
                            {
                                string entityName = reader.GetString(0);
                                Console.WriteLine("- " + entityName);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al listar las entidades: " + ex.Message);
            }
        }
        static void ListarAtributosEntidad()
        {
            try
            {

                using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
                {
                    connection.Open();

                    // Consulta SQL para obtener la lista de entidades de la base de datos
                    string query = "SELECT tablename FROM pg_catalog.pg_tables WHERE schemaname = 'public'";

                    Dictionary<int, string> entityDictionary = new Dictionary<int, string>();

                    using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                    {
                        using (NpgsqlDataReader reader = command.ExecuteReader())
                        {
                            Console.WriteLine("Lista de entidades en la base de datos:");

                            // Iterar sobre los resultados y mostrar el nombre de cada entidad con un ID asignado
                            int entityId = 1;
                            while (reader.Read())
                            {
                                string entityName = reader.GetString(0);
                                Console.WriteLine($"{entityId}. {entityName}");
                                entityDictionary.Add(entityId, entityName);
                                entityId++;
                            }
                        }
                    }

                    Console.Write("Seleccione el número de la entidad para ver sus atributos (o 0 para salir): ");
                    if (int.TryParse(Console.ReadLine(), out int selectedEntityId) && entityDictionary.ContainsKey(selectedEntityId))
                    {
                        string selectedEntity = entityDictionary[selectedEntityId];
                        MostrarAtributosEntidad(connection, selectedEntity);
                    }
                    else
                    {
                        Console.WriteLine("Selección no válida o no seleccionada. Saliendo del programa.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al listar los atributos de las entidades: " + ex.Message);
            }
        }
        static void MostrarAtributosEntidad(NpgsqlConnection connection, string tableName)
        {
            try
            {
                // Consulta SQL para obtener los atributos de una entidad específica
                string query = $"SELECT column_name FROM information_schema.columns WHERE table_name = '{tableName}'";

                using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                {
                    using (NpgsqlDataReader reader = command.ExecuteReader())
                    {
                        Console.WriteLine($"Atributos de la entidad '{tableName}':");

                        // Iterar sobre los resultados y mostrar el nombre de cada atributo
                        while (reader.Read())
                        {
                            string attributeName = reader.GetString(0);
                            Console.WriteLine("- " + attributeName);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al obtener los atributos de la entidad: " + ex.Message);
            }
        }
        static void AgregarEntidadConAtributos()
        {
            try
            {
                Console.Write("Ingrese el nombre de la entidad: ");
                string nombreEntidad = Console.ReadLine();

                Console.WriteLine("Ingrese los atributos de la entidad (separados por coma): ");
                string atributosInput = Console.ReadLine();
                string[] atributos = atributosInput.Split(',');

                using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
                {
                    connection.Open();

                    // Verificar si la entidad ya existe en la base de datos
                    string verificarEntidadQuery = $"SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = 'public' AND table_name = @nombreEntidad";
                    using (NpgsqlCommand verificarEntidadCmd = new NpgsqlCommand(verificarEntidadQuery, connection))
                    {
                        verificarEntidadCmd.Parameters.AddWithValue("@nombreEntidad", nombreEntidad);
                        int count = Convert.ToInt32(verificarEntidadCmd.ExecuteScalar());
                        if (count > 0)
                        {
                            Console.WriteLine($"La entidad '{nombreEntidad}' ya existe en la base de datos.");
                            return;
                        }
                    }

                    // Consulta SQL para agregar una nueva tabla
                    string createTableQuery = $"CREATE TABLE {nombreEntidad} (ID SERIAL PRIMARY KEY";
                    foreach (var atributo in atributos)
                    {
                        createTableQuery += $", {atributo} VARCHAR(50)";
                    }
                    createTableQuery += ")";

                    using (NpgsqlCommand command = new NpgsqlCommand(createTableQuery, connection))
                    {
                        command.ExecuteNonQuery();
                        Console.WriteLine($"Se ha creado la nueva entidad '{nombreEntidad}' correctamente.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al agregar la nueva entidad con atributos: " + ex.Message);
            }
        }
        static void GenerarInformePDF()
        {
            string rutaPDF = "C:/Users/melli/Downloads/Informe.pdf"; // Ruta para el archivo PDF
            try
            {
                using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
                {
                    connection.Open();
                    // Consulta SQL para obtener la lista de tablas en el esquema public
                    string query = "SELECT tablename FROM pg_catalog.pg_tables WHERE schemaname = 'public' ORDER BY tablename";

                    List<string> entities = new List<string>();

                    using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
                    using (NpgsqlDataReader reader = command.ExecuteReader())
                    {
                        Console.WriteLine("Entidades disponibles:");
                        int entityId = 1;
                        while (reader.Read())
                        {
                            string tableName = reader.GetString(0);
                            entities.Add(tableName);
                            Console.WriteLine($"{entityId}. {tableName}");
                            entityId++;
                        }
                    }

                    // Permitir al usuario la selección de la entidad
                    Console.Write("Seleccione el número de la entidad que desea mostrar en el informe: ");
                    if (int.TryParse(Console.ReadLine(), out int selectedEntityId) && selectedEntityId > 0 && selectedEntityId <= entities.Count)
                    {
                        string selectedEntity = entities[selectedEntityId - 1];

                        // Consulta SQL para obtener los atributos de la entidad seleccionada
                        string attributeQuery = "SELECT column_name FROM information_schema.columns WHERE table_schema = 'public' AND table_name = @TableName";

                        List<string> attributes = new List<string>();

                        using (NpgsqlCommand attributeCommand = new NpgsqlCommand(attributeQuery, connection))
                        {
                            attributeCommand.Parameters.AddWithValue("@TableName", selectedEntity);

                            using (NpgsqlDataReader attributeReader = attributeCommand.ExecuteReader())
                            {
                                // Mostrar los atributos disponibles para seleccionar
                                Console.WriteLine($"Atributos disponibles para la entidad '{selectedEntity}':");
                                while (attributeReader.Read())
                                {
                                    string attributeName = attributeReader.GetString(0);
                                    attributes.Add(attributeName);
                                    Console.WriteLine(attributeName);
                                }
                            }
                        }
                        Console.WriteLine("Ingrese los nombres de los atributos que desea mostrar (separados por coma):");
                        string selectedAttributesInput = Console.ReadLine();
                        string[] selectedAttributes = selectedAttributesInput.Split(',');

                        // Verificar que los atributos seleccionados son válidos
                        foreach (var attr in selectedAttributes)
                        {
                            if (!attributes.Contains(attr.Trim()))
                            {
                                Console.WriteLine($"El atributo '{attr.Trim()}' no es válido para la entidad '{selectedEntity}'.");
                                return;
                            }
                        }

                        // Crear la consulta SQL para obtener los datos de los atributos seleccionados
                        string columns = string.Join(", ", selectedAttributes.Select(attr => attr.Trim()));
                        string dataQuery = $"SELECT {columns} FROM {selectedEntity}";

                        List<Dictionary<string, object>> data = new List<Dictionary<string, object>>();

                        using (NpgsqlCommand dataCommand = new NpgsqlCommand(dataQuery, connection))
                        using (NpgsqlDataReader dataReader = dataCommand.ExecuteReader())
                        {
                            // Leer los datos y almacenarlos en una lista de diccionarios
                            while (dataReader.Read())
                            {
                                Dictionary<string, object> row = new Dictionary<string, object>();
                                foreach (var attr in selectedAttributes)
                                {
                                    row[attr.Trim()] = dataReader[attr.Trim()];
                                }
                                data.Add(row);
                            }
                        }

                        // Mostrar el informe en pantalla
                        Console.WriteLine($"\nInforme de la entidad: {selectedEntity}");
                        foreach (var row in data)
                        {
                            foreach (var kvp in row)
                            {
                                Console.WriteLine($"{kvp.Key}: {kvp.Value}");
                            }
                            Console.WriteLine();
                        }

                        // Preguntar al usuario si desea guardar el informe en PDF
                        Console.WriteLine("¿Desea guardar el informe en PDF? (s/n):");
                        string savePdfResponse = Console.ReadLine();

                        if (savePdfResponse.ToLower() == "s")
                        {
                            // Crear un nuevo documento PDF
                            using (PdfWriter writer = new PdfWriter(rutaPDF))
                            using (PdfDocument pdf = new PdfDocument(writer))
                            using (Document document = new Document(pdf))
                            {
                                document.Add(new Paragraph($"Entidad: {selectedEntity}"));

                                // Agregar los datos de los atributos seleccionados de la entidad al documento PDF
                                foreach (var row in data)
                                {
                                    foreach (var kvp in row)
                                    {
                                        document.Add(new Paragraph($"{kvp.Key}: {kvp.Value}"));
                                    }
                                    document.Add(new Paragraph("\n"));
                                }
                            }

                            Console.WriteLine("Informe PDF generado correctamente.");
                        }
                        else
                        {
                            Console.WriteLine("El informe no se guardó en PDF.");
                        }
                    }
                    else
                    {
                        Console.WriteLine("Selección no válida. Saliendo del programa.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al generar el informe: " + ex.Message);
            }
        }
        static void GenerarProcedimientosAlmacenados()
        {
            try
            {
                using (NpgsqlConnection connection = new NpgsqlConnection(connectionString))
                {
                    connection.Open();

                    // Consulta SQL para obtener el nombre de todas las tablas en el esquema public
                    string tableQuery = "SELECT tablename FROM pg_catalog.pg_tables WHERE schemaname = 'public'";

                    List<string> tableNames = new List<string>();

                    using (NpgsqlCommand tableCommand = new NpgsqlCommand(tableQuery, connection))
                    using (NpgsqlDataReader tableReader = tableCommand.ExecuteReader())
                    {
                        // Obtener los nombres de las tablas
                        while (tableReader.Read())
                        {
                            tableNames.Add(tableReader.GetString(0));
                        }
                    }

                    // Mostrar la lista de tablas disponibles
                    Console.WriteLine("Tablas disponibles:");
                    for (int i = 0; i < tableNames.Count; i++)
                    {
                        Console.WriteLine($"{i + 1}. {tableNames[i]}");
                    }

                    // Solicitar al usuario que seleccione una tabla
                    Console.Write("Ingrese el número de la tabla para generar los procedimientos almacenados: ");
                    if (int.TryParse(Console.ReadLine(), out int selectedTableIndex) && selectedTableIndex > 0 && selectedTableIndex <= tableNames.Count)
                    {
                        string selectedTable = tableNames[selectedTableIndex - 1];

                        // Generar los procedimientos almacenados para la tabla seleccionada
                        StringBuilder sb = new StringBuilder();

                        // Código para generar los procedimientos almacenados
                        // ...
                        sb.AppendLine($"-- Procedimientos almacenados para la tabla {selectedTable}");
                        sb.AppendLine();

                        // Procedimiento almacenado para INSERT
                        sb.AppendLine($"CREATE OR REPLACE PROCEDURE Insertar_{selectedTable}(");
                        sb.AppendLine(string.Join(", ", GetParameterList(connection, selectedTable)));
                        sb.AppendLine(")");
                        sb.AppendLine("LANGUAGE plpgsql AS $$");
                        sb.AppendLine("BEGIN");
                        sb.AppendLine($"    INSERT INTO {selectedTable} (");

                        sb.AppendLine(string.Join(", ", GetColumnList(connection, selectedTable)));

                        sb.AppendLine("    )");
                        sb.AppendLine("    VALUES (");

                        sb.AppendLine(string.Join(", ", GetParameterList(connection, selectedTable).Select(p => $"        {p}")));
                        sb.AppendLine("    );");
                        sb.AppendLine("END;");
                        sb.AppendLine("$$;");
                        sb.AppendLine();

                        // Procedimiento almacenado para UPDATE
                        string idColumn = GetIdColumn(connection, selectedTable);
                        sb.AppendLine($"CREATE OR REPLACE PROCEDURE Actualizar_{selectedTable}(");
                        sb.AppendLine(string.Join(", ", GetParameterList(connection, selectedTable)) + $", IN p_{idColumn} {GetColumnType(connection, selectedTable, idColumn)}");
                        sb.AppendLine(")");
                        sb.AppendLine("LANGUAGE plpgsql AS $$");
                        sb.AppendLine("BEGIN");
                        sb.AppendLine($"    UPDATE {selectedTable}");
                        sb.AppendLine("    SET");

                        sb.AppendLine(string.Join(", ", GetUpdateSetList(connection, selectedTable)));

                        sb.AppendLine($"    WHERE {idColumn} = p_{idColumn};");
                        sb.AppendLine("END;");
                        sb.AppendLine("$$;");
                        sb.AppendLine();

                        // Procedimiento almacenado para DELETE
                        sb.AppendLine($"CREATE OR REPLACE PROCEDURE Eliminar_{selectedTable}(IN p_{idColumn} {GetColumnType(connection, selectedTable, idColumn)})");
                        sb.AppendLine("LANGUAGE plpgsql AS $$");
                        sb.AppendLine("BEGIN");
                        sb.AppendLine($"    DELETE FROM {selectedTable}");
                        sb.AppendLine($"    WHERE {idColumn} = p_{idColumn};");
                        sb.AppendLine("END;");
                        sb.AppendLine("$$;");
                        sb.AppendLine();

                        // Procedimiento almacenado para SELECT
                        sb.AppendLine($"CREATE OR REPLACE PROCEDURE Seleccionar_{selectedTable}()");
                        sb.AppendLine("LANGUAGE plpgsql AS $$");
                        sb.AppendLine("BEGIN");
                        sb.AppendLine($"    SELECT * FROM {selectedTable};");
                        sb.AppendLine("END;");
                        sb.AppendLine("$$;");
                        sb.AppendLine();
                        // Guardar los procedimientos almacenados en un archivo
                        string downloadsFolder = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile) + @"\Downloads";
                        string filePath = Path.Combine(downloadsFolder, $"ProcedimientosAlmacenados_{selectedTable}.sql");

                        File.WriteAllText(filePath, sb.ToString());
                        Console.WriteLine($"Los procedimientos almacenados para la tabla '{selectedTable}' se han guardado en '{filePath}'.");
                    }
                    else
                    {
                        Console.WriteLine("Selección no válida.");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error al generar los procedimientos almacenados: " + ex.Message);
            }
        }

        static List<string> GetColumnList(NpgsqlConnection connection, string tableName)
        {
            List<string> columns = new List<string>();

            // Consulta SQL para obtener los nombres de las columnas de la tabla
            string columnQuery = $"SELECT column_name FROM information_schema.columns WHERE table_name = '{tableName}'";

            using (NpgsqlCommand columnCommand = new NpgsqlCommand(columnQuery, connection))
            using (NpgsqlDataReader columnReader = columnCommand.ExecuteReader())
            {
                while (columnReader.Read())
                {
                    columns.Add(columnReader.GetString(0));
                }
            }

            return columns;
        }

        static List<string> ListarEntidadesConAtributos(NpgsqlConnection connection)
        {
            List<string> entidades = new List<string>();
            string query = "SELECT tablename FROM pg_catalog.pg_tables WHERE schemaname = 'public'";

            using (NpgsqlCommand command = new NpgsqlCommand(query, connection))
            using (NpgsqlDataReader reader = command.ExecuteReader())
            {
                while (reader.Read())
                {
                    string tableName = reader.GetString(0);
                    entidades.Add(tableName);
                }
            }

            int index = 1;
            foreach (string tableName in entidades)
            {
                Console.WriteLine($"{index}. {tableName}");
                ListarAtributos(connection, tableName);
                index++;
            }

            return entidades;
        }

        static string GetIdColumn(NpgsqlConnection connection, string tableName)
        {
            // Consulta SQL para obtener el nombre de la columna de la clave primaria
            string idQuery = $@"
                SELECT kcu.column_name
                FROM information_schema.table_constraints tc
                JOIN information_schema.key_column_usage kcu
                ON tc.constraint_name = kcu.constraint_name
                AND tc.table_name = kcu.table_name
                WHERE tc.constraint_type = 'PRIMARY KEY' AND tc.table_name = '{tableName}';";

            using (NpgsqlCommand idCommand = new NpgsqlCommand(idQuery, connection))
            {
                return idCommand.ExecuteScalar().ToString();
            }
        }

        static List<string> GetParameterList(NpgsqlConnection connection, string tableName)
        {
            List<string> parameters = new List<string>();

            // Obtener los nombres de las columnas de la tabla
            List<string> columns = GetColumnList(connection, tableName);

            foreach (var column in columns)
            {
                parameters.Add($"IN p_{column} {GetColumnType(connection, tableName, column)}");
            }

            return parameters;
        }

        static List<string> GetUpdateSetList(NpgsqlConnection connection, string tableName)
        {
            List<string> updateSetList = new List<string>();

            List<string> columns = GetColumnList(connection, tableName);

            foreach (var column in columns)
            {
                updateSetList.Add($"        {column} = p_{column}");
            }

            return updateSetList;
        }

        static string GetColumnType(NpgsqlConnection connection, string tableName, string columnName)
        {
            // Consulta SQL para obtener el tipo de datos de la columna
            string columnTypeQuery = $"SELECT data_type FROM information_schema.columns WHERE table_name = '{tableName}' AND column_name = '{columnName}'";

            using (NpgsqlCommand columnTypeCommand = new NpgsqlCommand(columnTypeQuery, connection))
            {
                return columnTypeCommand.ExecuteScalar().ToString();
            }
        }

    }

}
