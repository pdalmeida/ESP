import base64
import sqlalchemy
from sqlalchemy import update
from google.cloud import iot_v1


def hello_pubsub(event, context):
    pubsub_message = base64.b64decode(event['data']).decode('utf-8')

    client = iot_v1.DeviceManagerClient()
    device_path = client.device_path('teste-esp8266-lucas', 'europe-west1', 'atest-registry', 'atest-dev')
    
    db_user = 'root' #  os.environ.get("DB_USER")
    db_pass = '123456789' #  os.environ.get("DB_PASS")
    db_name = 'wifistr' #  os.environ.get("DB_NAME")
    cloud_sql_connection_name = 'teste-esp8266-lucas:europe-west1:atest-db' #  os.environ.get("CLOUD_SQL_CONNECTION_NAME")
    
    db = sqlalchemy.create_engine(
        sqlalchemy.engine.url.URL(
            drivername='mysql+pymysql',
            username=db_user,
            password=db_pass,
            database=db_name,
            query={
                'unix_socket': '/cloudsql/{}'.format(cloud_sql_connection_name)
            },
        ),
    )
    
    mydict = {}
    mydict = eval(pubsub_message)
    action = list(mydict.values())[0]   

    if action == "request":
        command = 'Hello IoT Core!'
        data = command.encode("utf-8")
        return client.send_command_to_device(request={"name": device_path, "binary_data": data})

    else:
        columns = ', '.join("`" + str(x).replace('/', '_') + "`" for x in list(mydict.keys())[1:])
        values = ', '.join("'" + str(x).replace('/', '_') + "'" for x in list(mydict.values())[1:])
        sql = "INSERT INTO %s ( %s ) VALUES ( %s );" % ('first', columns, values)

    # stmt = sqlalchemy.text('INSERT INTO wifistr(wifistr) VALUES (:data)')

    try:
        with db.connect() as conn:
            conn.execute(sql, data=pubsub_message)
        message.ack()
    except Exception as e:
        print(e)