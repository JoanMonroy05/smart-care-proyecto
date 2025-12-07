let mq3Value, tempValue, pulseValue, spo2Value;

document.addEventListener('DOMContentLoaded', function() {
    mq3Value = document.getElementById('mq3Value');
    tempValue = document.getElementById('tempValue');
    spo2Value = document.getElementById('spo2Value');
    
    // Iniciar actualización (una sola petición)
    actualizarMediciones();
    setInterval(actualizarMediciones, 2000);
});

function animar(cardId) {
    const card = document.getElementById(cardId);
    if (card) {
        card.classList.add("updated");
        setTimeout(() => card.classList.remove("updated"), 300);
    }
}

async function actualizarMediciones() {
    try {
        // ✅ UNA SOLA petición para todo
        const response = await fetch("/api/sensores");
        const data = await response.json();
        
        // Actualizar MQ3
        if (mq3Value) {
            mq3Value.innerText = data.alcohol.toFixed(2) > 0 ? data.alcohol.toFixed(2) + " mg/L" : "0 mg/L";
            animar("card_mq3");
        }
        
        // Actualizar Temperatura
        if (tempValue) {
            tempValue.innerText = data.temperatura.toFixed(1) > 0 ? data.temperatura.toFixed(1) + " °C" : "0 °C";
            animar("card_temp");
        }
        
        // Actualizar SpO2
        if (spo2Value) {
            spo2Value.innerText = data.spo2 > 0 ? data.spo2 + " %" : "0 BPM";
            animar("card_spo2");
        }
        
    } catch (err) {
        console.error('Error obteniendo datos:', err);
    }
}