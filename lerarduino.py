import  serial
import  pymysql.connections 
import  time
from decimal import  Decimal 

arduino = serial.Serial('/dev/ttyUSB0', 9600)

while True:
    linharecebida = arduino.readline().decode('utf-8').strip()
    #print(linharecebida[2:-5])
    print(linharecebida)
    linha=linharecebida[27:]
    linha=linha.strip()
    print(linha)
    print("A linha tem",len(linha),"caracteres")
    #print("Dado inválido recebido:", linharecebida)
    print(f"Valor lido: {linha}")
    linha = Decimal(linha)

    try:
        connection = pymysql.Connect(
            host= 'localhost',
            database = 'arduino',
            user = 'gerente',
            password = '******@02'
        )
        query = "INSERT INTO temperatura (temp) VALUES (%s)"
        cursor = connection.cursor()
        cursor.execute(query,(linha,))
        connection.commit()
        query_consulta = "SELECT COUNT(*) FROM temperatura"
        cursor.execute(query_consulta)
        total = cursor.fetchone()[0]
        print(cursor.rowcount," registro Inserido com valor: ",linha)
        print("Total de registros: ", total)

    except pymysql.Connection.Error as error:
        print("Erro na conexão")
        print(error)
    finally:
        if 'cursor' in locals():
                cursor.close()
        if 'connection' in locals() and connection.open:
                connection.close()
                print("Banco fechado")
    time.sleep(10)
#Umidade: 47.00 Temperatura: 28.60  
arduino.close

