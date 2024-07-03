CREATE TABLE a_prueba (
    id SERIAL PRIMARY KEY,
    nombre_tabla VARCHAR(255) NOT NULL,
    fecha_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    usuario_actual VARCHAR(255) NOT NULL,
    detalle_accion TEXT NOT NULL
);

CREATE TABLE auditoria (
    id SERIAL PRIMARY KEY,
    nombre_tabla VARCHAR(255),
    operacion VARCHAR(10),
    usuario_actual VARCHAR(255),
    fecha_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    detalle_anterior TEXT,
    detalle_nuevo TEXT
);



select * from a_prueba
select * from auditoria

------------------------------------------------------------------------------
INSERT INTO a_prueba (nombre_tabla, usuario_actual, detalle_accion)
VALUES ('prueba1', 'BORYS', 'prueba');

------------------------------------------------------------------------------
UPDATE a_prueba
SET nombre_tabla = 'nuevo_nombre_tabla',
    usuario_actual = 'nuevo_usuario',
    detalle_accion = 'Nuevo detalle de acción'
WHERE id = 1; 

------------------------------------------------------------------------------

DELETE FROM a_prueba
WHERE id = 3;
