#ifndef INDEX_H
#define INDEX_H

const char* html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>18650 Battery Tester</title>
		<link href="https:<!--fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet">
		<h1>18650 Battery Tester</h1>
		<div class="container">
		<div class='gridTop'>
			<div class="slider-container">
				<label for="enableSlider">Enable:</label><br>
				<label class="switch">
				<input type="checkbox" id="enableSlider">
				<span class="slider"></span>
				</label>
                <h2></h2>
                <button onclick="debug()" class="button">Print out data</button> <!-- DEBUG(CELL) how do i add active cell in this-->
			</div>
			
			<div id="terminal"><!-- Terminal output window 
				<div class="log debug">logMessage('debug', 'This is a debug message')</div>
				<div class="log info">[INFO]: This is an info message</div>
				<div class="log warning">[WARNING]: This is a warning message</div>-->
			</div>
            <div class="cellDcload">
				<h2>DC Load Settings</h2>
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Address:</span>
                    <input type="text" placeholder="DC Load IP Address" id="dcLoadIP">
                    <span class="suffix"> </span>
                </div>
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Port:</span>
                    <input type="text" placeholder="DC Load Port" id="dcLoadPort">
                    <span class="suffix"> </span>
                </div>

				<h1></h1>
				
			    <div class="gridButton">
                    <div class="cell"></div>
                    <div class="cell">
                    <button onclick="connectDCLoad()" class="buttonSmall">Connect to DC Load</button>  
                    </div>
                    <div class="cell"></div>
                </div>
            </div>
		</div>
		<!-- Relays will be rendered dynamically -->
		<div id="relayGrid" class="grid"></div>
		
		<div class='gridField'>

            <!-- Global Settings Section -->
            <div class="cell">
                <h3>Global Settings</h3>
        
                <!-- Cell Capacity Input with Prefix and Suffix -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Cell:</span>
                    <input type="text" placeholder="Capacity" id="CellCapacity">
                    <span class="suffix">mAh</span>
                </div>
        
                <!-- Sweep Cell Delay Input with Suffix -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Sweep delay</span>
                    <input type="text" placeholder="Sweep Delay" id="sweepCellDelay">
                    <span class="suffix">Sec</span>
                </div>
        
                <!-- Test Mode Delay Input with Suffix -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Testmode delay</span>
                    <input type="text" placeholder="Mode Delay" id="testModeDelay">
                    <span class="suffix">Sec</span>
                </div>
        
                <!-- Read Voltage Offset Input with Suffix -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Sample offset</span>
                    <input type="text" placeholder="Volt Offset" id="readVoltOffset">
                    <span class="suffix">Sec</span>
                </div>
            </div>
        
            <!-- Nominal Settings Section -->
            <div class="cell">
                <h3>Nominal Settings</h3>
                
                <!-- Duration Input with Unit -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Discharge Time</span>
                    <input type="text" placeholder="Duration (s)" id="duration0">
                    <span class="suffix">Sec</span>
                </div>
        
                <!-- Discharge Input with Unit -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Discharge current</span>
                    <input type="text" placeholder="Discharge (C)" id="discharge0">
                    <span class="suffix">[C]</span>
                </div>
            </div>
        
            <!-- Continuous DCHG Section -->
            <div class="cell">
                <h3>Continuous DCHG</h3>
        
                <!-- Duration Input with Unit -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Discharge Time</span>
                    <input type="text" placeholder="Duration (Sec)" id="duration1">
                    <span class="suffix">Sec</span>
                </div>
        
                <!-- Discharge Input with Unit -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Discharge current</span>
                    <input type="text" placeholder="Discharge (C)" id="discharge1">
                    <span class="suffix">[C]</span>
                </div>
            </div>
        
            <!-- Burst DCHG Section -->
            <div class="cell">
                <h3>Burst DCHG</h3>
        
                <!-- Duration Input with Unit -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Discharge Time</span>
                    <input type="text" placeholder="Duration (Sec)" id="duration2">
                    <span class="suffix">Sec</span>
                </div>
        
                <!-- Discharge Input with Unit -->
                <div class="input-with-prefix-suffix">
                    <span class="prefix">Discharge current</span>
                    <input type="text" placeholder="Discharge (C)" id="discharge2">
                    <span class="suffix">[C]</span>
                </div>
            </div>

            <div>
                <button  onclick="fullSweep()" class="buttonSmall">Full Sweep</button>
                <button onclick="saveAll()" class="button">Save for active cell</button>  <!-- Want Save for cell n-->
                <button onclick="saveGlobalSettings()" class="button">Save for all cells</button>
                
                
            </div>
        
        </div>
        
            <style>
                /* Align body contents to center */
                body {
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    background-color: #1E1E2F;
                    color: #FFFFFF;
                    flex-direction: column;
                    font-family: 'Roboto', sans-serif;
                    margin: 0;
                    padding: 0;
                }
        
                 /* Style for the message terminal window */
                #terminal {
                    grid-column: 2 / span 2; /* Make the terminal span across both columns */
                    grid-row: 1 / 4; /* Place terminal in the second row */
                    max-height: 15vh; /* Set the maximum height to 50% of the viewport */
                    background-color: black;
                    color: green;
                    font-family: monospace;
                    padding: 10px;
                    overflow-y: auto;
                    white-space: pre-wrap;
                    border: 1px solid #333;
                    margin-top: 10px;
                    min-height: 200px;
                }
                
                /* Container for input with prefix and suffix */
            .input-with-prefix-suffix {
                    display: flex;              /* Align items horizontally */
                    align-items: center;        /* Vertically center items */
                    margin-bottom: 10px;        /* Add space between each input group */
                }

                /* Style for the prefix and suffix */
                .input-with-prefix-suffix .prefix,
                .input-with-prefix-suffix .suffix {
                    font-size: 14px;            /* Set the font size */
                    color: #bbb7f7;                /* Slightly gray color for the text */
                    margin: 0 8px;              /* Add horizontal margin to separate from input */
                }

                /* Style for the input fields */
                .input-with-prefix-suffix input {
                    width: 150px;               /* Fixed width for input fields */
                    padding: 5px;               /* Padding inside the input */
                    margin: 0;                  /* Ensure no default margin */
                }

                /* Styling for card components */
                .card {
                    background-color: #2E2E3F;
                    border: 1px solid #5b5b99;
                    border-radius: 5px;
                    padding: 15px;
                }
        
                /* Style for cell elements (relay sections) */
                .cell {
                    align-items: center;
                    background-color: #2E2E3F;
                    border: 0px solid #5b5b99;
                    border-radius: 10px;
                    padding: 2px;
                    flex-direction: column;
                    justify-content: space-between;
                    height: 100%;
                }
        
                /* Style for DC Load Placement */
                .cellDcload {
                    grid-column: 4; /* Place the button in the fourth column */
                    grid-row: 1; /* Place it in the first row */
                    align-items: center;
                }
        
                /* General container style */
                .container {
                    background-color: #2A2A3B;
                    border-radius: 10px;
                    box-shadow: 0 4px 15px rgba(0, 0, 0, 0.5);
                    max-width: 1140px;
                    min-width: 1200px;
                    padding: 50px;
                    width: 90%;
                }
        
                /* Activate relay button style */
                .activate-relay {
                    background-color: #4CAFED;
                    border: none;
                    border-radius: 250px;
                    color: white;
                    cursor: pointer;
                    font-size: 18px;
                    margin-bottom: 30px;
                    padding: 30px;
                    width: 100%;
                }
        
                /* Button styles */
                button {
                    background-color: #4CAFED;
                    align-items: center;
                    border: none;
                    border-radius: 5px;
                    color: white;
                    cursor: pointer;
                    margin: 5px 0;
                    padding: 5px;
                    width: 45%;
                }
        
                /* Button hover effect */
                button:hover {
                    background-color: rgb(255, 0, 85);
                }
        
                /* Small button styles */
                .buttonSmall {
                    background-color: #4CAFED;
                    align-items: center;
                    border: 1px;
                    border-radius: 10px;
                    color: white;
                    cursor: pointer;
                    margin: 5px 0;
                    padding: 5px;
                    width: 100%;
                }
        
                /* Small button hover effect */
                buttonSmall:hover {
                    background-color: #277db3;
                }
        
                /* Grid layout for fields */
                .gridField {
                    grid-template-columns: repeat(auto-fill, minmax(150px, 1fr));
                    display: grid;
                    gap: 80px;
                }
        
                /* Grid layout for relays */
                .grid {
                    grid-template-columns: repeat(auto-fill, minmax(230px, 1fr));
                    display: grid;
                    gap: 50px;
                    padding: 50px;
                }
        
                /* Button grid layout */
                .gridButton {
                    grid-template-columns: 30% 40% 30%;
                    align-items: center;
                    display: grid;
                    gap: 5px;
                    width: 100%;
                }
        
                /* Layout for top-level controls */
                .gridTop {
                    grid-template-columns: 250px 1fr 1fr 250px;
                    align-items: center;
                    display: grid;
                    gap: 30px;
                    width: 100%;
                    max-width: 100%; /* Ensure the grid doesn't overflow the container */
                }
        
                /* Header styles */
                h1 {
                    color: #4CAFED;
                    text-align: center;
                }
        
                /* Subheader styles */
                h2 {
                    color: #4CAFED;
                    font: bold;
                    font-size: 22px;
                    text-align: center;
                }
        
                /* Section title styles */
                h3 {
                    color: white;
                    font-size: 18px;
                    text-align: left;
                }
        
                /* Style for the header section (can be any other content) */
                .header {
                    background-color: #444;
                    color: white;
                    padding: 10px;
                    text-align: center;
                }
        
                /* Input text field styling */
                input[type="text"] {
                    background-color: #2E2E3F;
                    border: 1px solid #4CAFED;
                    border-radius: 8px;
                    color: #FFFFFF;
                    padding: 8px;
                    width: calc(100% - 20px);
                }
        
                /* Slider container styles */
                .slider-container {
                    margin-bottom: 20px;
                    text-align: center;
                }
        
                /* Slider label styling */
                .slider-container label {
                    color: #FFFFFF;
                    font-size: 1.2em;
                }
        
                /* Slider styles */
                .switch {
                    display: inline-block;
                    height: 40px; /* Increased height */
                    position: relative;
                    width: 80px; /* Increased width */
                }
        
                /* Hide default checkbox */
                .switch input {
                    height: 0;
                    opacity: 0;
                    width: 0;
                }
        
                /* Slider background and transitions */
                .slider {
                    background: linear-gradient(45deg, #1d5274, #4CAFED); /* Gradient background */
                    border-radius: 40px; /* Rounded corners */
                    bottom: 0;
                    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.3); /* Subtle shadow */
                    cursor: pointer;
                    left: 0;
                    position: absolute;
                    right: 0;
                    top: 0;
                    transition: background 0.4s, box-shadow 0.3s; /* Smooth transitions */
                }
        
                /* Slider knob styles */
                .slider:before {
                    background-color: #2E2E3F;
                    border-radius: 50%; /* Circle */
                    box-shadow: 0 3px 6px rgba(0, 0, 0, 0.2); /* Knob shadow */
                    bottom: 4px;
                    content: "";
                    height: 32px; /* Adjusted size */
                    left: 4px;
                    position: absolute;
                    transition: transform 0.4s, box-shadow 0.3s; /* Smooth movement */
                    width: 32px;
                }
        
                /* Styles when slider is checked */
                input:checked + .slider {
                    background: linear-gradient(90deg, #4CAF50, #81C784); /* Gradient for "ON" */
                    box-shadow: 0 0 15px rgba(76, 175, 80, 0.6); /* Glow effect */
                }
        
                /* Move knob to the right when checked */
                input:checked + .slider:before {
                    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3); /* Enhance shadow */
                    transform: translateX(40px); /* Smooth transition */
                }
        
                /* General log styling */
                .log {
                    margin: 2px 0;
                    padding: 0px;
                    border-radius: 4px;
                }    
                
                 /* Voltage Card */
                 .voltage-row {
                    
                    margin: 2px 0;
                    padding: 0px;
                    border-radius: 4px;
                }

                /* Styling for single test section */
                .single-test {
                    margin-bottom: 20px;
                    margin-top: 20px;
                }
                
                /* Specific styling for each log level */
                .debug {
                    color: yellow; /* Yellow for debug messages */
                }
        
                .info {
                    color: white; /* White for info messages */
                }
        
                .warning {
                    color: red; /* Red for warning messages */
                }

                .active-relay {
                    background-color: green; /* Red for warning messages */
                }
            </style>                            
        </head>

        <body>
            <script>
            /_* Should be stored as an array i common code (Arduino) or are these even used ? */
            // Global variables to store data
                let activeCells = [false, false, false, false];
                var Voltages = [
                    [3.71, 3.65, 3.72],
                    [3.68, 3.60, 3.75],
                    [3.69, 3.64, 3.70],
                    [3.67, 3.61, 3.74]
                ];

                var EN = false;              // Boolean to store the state of the slider
                var DCLoadConnected = false; // Boolean to store if the DC load is connected
                var activeRelay = -1;      // Variable to track the currently active relay

                // Temporary test settings mirroring the backend structure
            const testSettings = [
                { cellCapacity: 3200, currentC0: 0.5, duration0: 10, currentC1: 2, duration1: 10, currentC2: 3, duration2: 1, readVoltOffset: 0.1 },
                { cellCapacity: 3200, currentC0: 0.5, duration0: 10, currentC1: 2, duration1: 10, currentC2: 3, duration2: 1, readVoltOffset: 0.1 },
                { cellCapacity: 3200, currentC0: 0.5, duration0: 10, currentC1: 2, duration1: 10, currentC2: 3, duration2: 1, readVoltOffset: 0.1 },
                { cellCapacity: 3200, currentC0: 0.5, duration0: 10, currentC1: 2, duration1: 10, currentC2: 3, duration2: 1, readVoltOffset: 0.1 }
            ];
            
            /* Dubble toggles for some reason also when enablked it says deactivated and no test can be done as if en is false , but its not*/  
            /* exept for fullsweep which says en deactivated, but nothing happens when EN = true.*/
            function enableSlider() {
                    EN = document.getElementById("enableSlider").checked;
                    //if (EN) EN = true; 
                    //else EN = false;
                    logMessage(EN ? 'warning' : 'debug' , EN ? '! Relays Activated !' : 'Relays Deactivated!');
                    console.log("Relays enabled:", EN);
                    document.querySelectorAll('.activate-relay, .buttonSmall').forEach(btn => {
                        btn.disabled = !EN;
                        });
                    document.querySelector('.slider').style.backgroundColor = EN ? "#4b58c7" : "#4CAFED";
            }


            /* Initialize the slider when the page loads */
            window.onload = () => {
                    const slider = document.getElementById("enableSlider");
                    slider.checked = EN; // Set initial state
                    slider.addEventListener("change", enableSlider);
            };


            /* Function to add messages to the terminal window */                
            function logMessage(level, message) {
                //let activeRelay = null;
                const terminal = document.getElementById('terminal');
                const logElement = document.createElement('div');
                if (terminal.children.length > 7) {
                    terminal.removeChild(terminal.lastChild);
                }
                logElement.classList.add('log', level); // Add both 'log' and the level class (debug, info, warning)
                logElement.textContent = `[${level.toUpperCase()}]: ${message}`;
                terminal.prepend(logElement);
                       
            }     
             
            /* */
                function activateRelay(button, relay) {

                    // Set the active relay to the selected relay number
                    const buttonsWithActiveClass = document.querySelectorAll('button.activate-relay');
                    buttonsWithActiveClass.forEach(btn => {
                        btn.classList.remove('active-relay');
                    });
                    button.classList.add('active-relay');
                    activeRelay = relay;

                    // Log and console message indicating the active cell
                    logMessage('info', `Cell #${relay} is now active.`);
                    console.log(`Cell #${relay} is now active.`);

                    // Send a request to the backend to activate the relay
                    fetch(`/activateRelay?relay=${relay}`)
                        .then((response) => {
                            if (!response.ok) throw new Error("Failed to activate relay");
                            return response.json();
                        })
                        .then((data) => {
                            if (data.success) {
                                updateRelayButtons();  // Update button colors to reflect the active relay
                                logMessage('info', `Relay ${relay} activated successfully.`);
                            } else {
                                alert("Failed to activate relay. Check EN state.");
                                logMessage('error', `Relay ${relay} activation failed.`);
                            }
                        })
                        .catch((err) => {
                            console.error(err);
                            logMessage('error', `Error: ${err.message}`);
                        })
                        .finally(() => {
                            // Call the backend function to update relays in the Arduino code
                            fetch(`/updateRelays?relay=${relay}`, {
                                method: 'POST',
                                headers: { 'Content-Type': 'application/json' },
                                body: JSON.stringify({ relay: relay })
                            })
                            .then((response) => {
                                if (!response.ok) throw new Error("Failed to run backend function updateRelays");
                                return response.json();
                            })
                            .then((data) => {
                                if (data.success) {
                                    logMessage('info', `Backend function updateRelays(${relay}) executed successfully.`);
                                    console.log(`Backend function updateRelays(${relay}) executed successfully.`);
                                    updateRelayButtons();
                                } else {
                                    logMessage('error', `Backend function updateRelays(${relay}) failed.`);
                                    console.error(`Backend function updateRelays(${relay}) failed.`);
                                }
                            })
                            .catch((err) => {
                                console.error(err);
                                logMessage('error', `Error calling updateRelays: ${err.message}`);
                            });
                        });
                }
 
            
                // Why dont i work ??
            function updateRelayButtons() {
                const buttons = document.querySelectorAll(".activate-relay");
                buttons.forEach((button, index) => {
                    if (index === activeRelay) {
                        button.style.backgroundColor = "green";
                    } else {
                        button.style.backgroundColor = "blue";
                    }
                });
            }

            function connectDCLoad() {
                console.log("Settingup DC Load");
                logMessage('info', 'Trying to connect to DC Load');
                // if (DCLoad.init()) logMessage('warning', 'Unable to connect to $dcLoadIP'); Connection to DCLoad is managed by web ui
            }
            
            function singleTest(cellIndex, testMode) {
                if (!readyToRun()) return;
                logMessage('info', "singel");
                console.log(`Single Test for Cell ${cellIndex}, Mode: ${testMode}`);
                runTest('single', cellIndex);
            }

            function fullSingleTest(relay) {
                if (!readyToRun()) return;
                logMessage('info', " - Full test on cell ${relay} ");
                runTest('fullSingle', relay);
            }

            function fullSweep() {
                if (!readyToRun()) return;
                logMessage('info', " - Full Sweep -");
                runTest('sweep');
            }

            function runTest(testMode, cellIndex = null) { // Add Test type also  .  but where is the test senarios taken from ???? 
                    if (!readyToRun()) return;

                    let message;
                    let current = Voltages[cellIndex - 1][0]; // Example: Using nominal voltage as current placeholder
                    let duration = 10; // Example: Hardcoded duration; replace with dynamic value if available

                    if (testMode === '0') {
                        console.log(`Single Test for Cell ${cellIndex}`);
                        message = `Running single test on Cell ${cellIndex} @ ${current.toFixed(2)}A / ${duration} seconds`;

                    } else if (testMode === '1') {
                        console.log(`Full Single Test for Relay ${cellIndex}`);
                        message = `Running all tests on Cell ${cellIndex} @ ${current.toFixed(2)}A / ${duration} seconds`;
                    } else if (testMode === 'sweep') {
                        console.log("Full 2 initiated");
                        message = 'Beginning sweeping cells 1-4. All tests';
                    }

                    logMessage('info', message);
                }

            function renderVoltageCards() {
                const grid = document.getElementById("grid");
                grid.innerHTML = ""; /* Clear current content */
                for (let relay = 0; relay < Voltages.length; relay++) {
                    const card = document.createElement("div");
                    card.className = "card";
                    card.innerHTML = `
                    <h3>Cell ${relay + 1}</h3>
                    <div class="voltage-row">
                        <span>Nominal Voltage:</span> <span>${Voltages[relay][0].toFixed(2)} V</span>
                    </div>
                    <div class="voltage-row">
                        <span>Max Discharge Voltage:</span> <span>${Voltages[relay][1].toFixed(2)} V</span>
                    </div>
                    <div class="voltage-row">
                        <span>Max Burst Voltage:</span> <span>${Voltages[relay][2].toFixed(2)} V</span>
                    </div>
                    `;
                    grid.appendChild(card);
                }
                }

                // If everything is in order tests can be initiated
                function readyToRun() {
                    if (!EN) {
                        logMessage('warning', 'Error: Cannot run tests while slider is off (DISABLED)');
                        return false;
                    }
                    return true;
                }

            function saveGlobalSettings() {
                    // Retrieve input values and parse them
                    let cellCapacity = parseInt(document.getElementById("CellCapacity").value) || 0;
                    let sweepCellDelay = parseInt(document.getElementById("sweepCellDelay").value) || 0;
                    let testModeDelay = parseInt(document.getElementById("testModeDelay").value) || 0;
                    let readVoltOffset = parseFloat(document.getElementById("readVoltOffset").value) || 0;

                    // Loop through the test settings and update each entry
                    for (let i = 0; i < 3; i++) {
                        let duration = parseFloat(document.getElementById(`duration${i}`).value) || 0;
                        let discharge = parseFloat(document.getElementById(`discharge${i}`).value) || 0;

                        testSettings[i].currentC = discharge;
                        testSettings[i].duration = duration;
                    }

                    // Update the testSettings array with the new values
                    testSettings[0].cellCapacity = cellCapacity;
                    testSettings[0].sweepCellDelay = sweepCellDelay;
                    testSettings[0].testModeDelay = testModeDelay;
                    testSettings[0].readVoltOffset = readVoltOffset;

                    // Log the updated settings
                    logMessage('debug', 'Global settings saved for active cell.');
                    logMessage('debug', 'Global Settings Saved:');
                    logMessage('debug', `Read Voltage Offset: ${readVoltOffset}`);
                    logMessage('debug', `Test Mode Delay: ${testModeDelay}`);
                    logMessage('debug', `Sweep Cell Delay: ${sweepCellDelay}`);
                    logMessage('debug', `Cell Capacity: ${cellCapacity}`);                  
                }

            
            function updateVoltages() {
                Voltages = Voltages.map(volts => volts.map(v => v + (Math.random() - 0.5) * 0.01));

                renderVoltageCards();
            }
            
            
            
            function renderRelays() {
                const relayGrid = document.getElementById("relayGrid");
                relayGrid.innerHTML = ""; /* Clear existing content */
                
                Voltages.forEach((volts, relay) => {
                    const relayDiv = document.createElement("div");
                    relayDiv.className = "cell";
                    
                    /* Construct the voltage rows */
                    const voltageRows = volts.map((voltage, index) => {
                        const labels = ["Nominal Voltage", "Max Discharge Voltage", "Max Burst Voltage"];
                        return `
                        <div style="display: flex; justify-content: space-between; margin-bottom: 6px;">
                            <span style="text-align: left;">${labels[index]}:</span>
                            <span style="text-align: right;">${voltage.toFixed(2)} V</span>
                        </div>
                        `;
                    }).join("");
                    
                    relayDiv.innerHTML = `
                    <button class="activate-relay" style="cell" onclick="activateRelay(this, ${relay})">Cell ${relay + 1}</button>
                    <div style="flex-grow: 1; display: flex; flex-direction: column; justify-content: space-between;">
                        <button class="buttonSmall" onclick="fullSingleTest(${relay + 1})">Full Test</button>
                        <button class="buttonSmall" onclick="singleTest(${relay + 1}, document.getElementById('testMode${relay + 1}').value)">Single Test</button>
                        <div class="single-test">
                            <select id="testMode${relay + 1}">
                                <option value="0">Nominal</option>
                                <option value="1">Const DCHG</option>
                                <option value="2">Burst DCHG</option> 
                            </select>
                        </div>
                    </div>
                    <div style="margin-top: 15px;">
                        ${voltageRows}
                    </div>
                `;

                    
                    relayGrid.appendChild(relayDiv);
                });

            }
            
            fetch('/activate', {
              method: 'POST',
              body: JSON.stringify({ relay: 1 }),
              headers: { 'Content-Type': 'application/json' }
            })
            .then(response => response.json())
            .then(data => {
            if (data.success) {
                updateUI(data.activeRelay);
            } else {
                alert(data.message); // Show an error message like "EN is false"
            }
            })
            .catch(error => {
                console.error('Error:', error);
                alert('An error occurred while activating the relay.');
            });

            
            document.addEventListener("DOMContentLoaded", () => {
                renderRelays();
                renderVoltageCards();
                setInterval(updateVoltages, 1000);   
            });
            
            function debug() {
                let activeCell = activeRelay + 1;
                logMessage('warning', 'END OF PRINTOUT');
                logMessage('info', '---------------'); 
                let testMode = parseInt(document.getElementById(`testMode${1}`).value);
                logMessage('debug', 'EN = ' + EN + '    activeRelay = ' + activeRelay + '     TestMode = ' + testMode);
                logMessage('debug', 'cellCapacity = ' +     testSettings[0].cellCapacity         + ' mAh    '   +
                                    'sweepCellDelay = ' +     testSettings[0].sweepCellDelay    + ' Sec'); 
                logMessage('debug', 'testModeDelay = ' +     testSettings[0].testModeDelay        + ' Sec   '    +
                                    'readVoltOffset = ' +     testSettings[0].readVoltOffset       + ' Sec');
                logMessage('info', '---------------');
                    // data is suposed to be taken from setSettings[3] where testSettings[1].duration
                if (testMode === 0) logMessage('debug', 'Nominal Settings Cell ' + activeCell + '    '+ testSettings[activeRelay].duration0 + ' Seconds   ' + testSettings[activeRelay].currentC0 + ' [C]');
                if (testMode === 1) logMessage('debug', 'DCHG Settings Cell ' + activeCell + '    '+ testSettings[activeRelay].duration1 + ' Seconds   ' + testSettings[activeRelay].currentC1 + ' [C]');
                if (testMode === 2) logMessage('debug', 'Burst Settings Cell ' + activeCell + '    '+ testSettings[activeRelay].duration2 + ' Seconds   ' + testSettings[activeRelay].currentC2 + ' [C]');
            }


            </script>
            </body>
</html>
)rawliteral";

#endif // INDEX_H