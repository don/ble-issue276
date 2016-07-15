var SERVICE = 'aa7b3c40-f6ed-4ffc-bc29-5750c59e74b3';
var CHARACTERISTIC = 'b0351694-25e6-4eb5-918c-ca9403ddac47';

var app = {
    initialize: function() {
        this.bindEvents();
    },
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },
    onDeviceReady: function() {
        app.scan();
    },
    scan: function() {
        var div = document.querySelector('div');
        div.innerText = "Scanning for " + SERVICE;
        
        var scanFailed = function() {
            div.innerHTML = "Scan failed";
        };
        
        var connectFailed = function() {
            div.innerHTML = "Failed to connect to device";            
        };
        
        // warning this scans forever if no devices are found
        var discoveredPeripheral = function(peripheral) {
            div.innerText = "Found " + peripheral.id;
            ble.stopScan();
            ble.connect(peripheral.id, app.onConnected, connectFailed);
        };
        
        ble.startScan([SERVICE], discoveredPeripheral, scanFailed);
    },
    onConnected: function(peripheral) {
        document.querySelector('div').innerText = "Connected " + peripheral.id;
        app.pre = document.querySelector('pre');
        var subscribeFailed = function() { pre.innerText = "Error subscribing for notifications"; };
        ble.startNotification(peripheral.id, SERVICE, CHARACTERISTIC, app.onData, subscribeFailed);

    },
    onData: function(buffer) {
        var value = new Uint8Array(buffer)[0];
        console.log(value);
        app.pre.innerText = value;
    }
    
};

app.initialize();

