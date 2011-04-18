global rolloutRornUtils

macroscript RornUtils category:"Tools"
(
	rollout rolloutRornUtils "Rorn Utils" width:392 height:103
	(
		groupBox grpModelExport "Model Export" pos:[3,3] width:386 height:164
			label lblExportPath "Export Path:" pos:[13,23] width:60 height:15
			editText editExportPath "" pos:[73,19] width:267 height:20
			button btnBrowseExportPath "..." pos:[347,19] width:30 height:20
			button btnExport "Export Full Scene" pos:[13,48] width:366 height:23
			button btnExportSelected "Export Selected Nodes Only" pos:[13,74] width:366 height:23
		
		on rolloutRornUtils open do
		(
			callbacks.addScript #filePostOpen "rolloutRornUtils.OnMyPostOpen()"

			editExportPath.text = "C:\\new_scene.xml"
			tempString = getAppData rootNode 0
			if tempString != undefined do
			(
				editExportPath.text = getAppData rootNode 0
			)
		)
		
		on rolloutRornUtils close do
		(
			callbacks.removeScripts #filePostOpen

			tempString = getAppData rootNode 0
			if tempString != editExportPath.text do
			(
				setAppData rootNode 0 editExportPath.text
			)
		)
		
		on btnExport pressed  do
		(
			oldExportPath = getAppData rootNode 0
			if oldExportPath != editExportPath.text do
			(
				setAppData rootNode 0 editExportPath.text
			)
			RornUtils_Export editExportPath.text
		)

		on btnBrowseExportPath pressed  do
		(
			newExportPath = getSaveFileName caption:"Select the save location" \
				filename:editExportPath.text types:"Rorn Scene(*.xml)|*.xml"
			if (newExportPath != undefined) do
			(
				editExportPath.text = newExportPath;
			)
		)

		fn OnMyPostOpen =
		(
			if rolloutRornUtils.open do
			(
				rolloutRornUtils.editExportPath.text = "C:\\new_scene.xml"
				tempString = getAppData rootNode 0
				if tempString != undefined do
				(
					editExportPath.text = getAppData rootNode 0
				)
			)
		)
	)

	on execute do
	(
		createDialog rolloutRornUtils 392 103
	)
)
